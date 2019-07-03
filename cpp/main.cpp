#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cmath>
#include "LSH.h"
#define INF 1000000000
using namespace std;

typedef long long int lli;

string get_string(string path){
    string ret;
    ifstream myfile(path);
    if(myfile.is_open()){
        string line;
        while(getline(myfile, line)){
            ret += line;
            ret += "\n";
        }
        myfile.close();
    }
    return ret;
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    srand(time(NULL));

    vector<string> files;
    files.push_back("C:/LSH_test/topolo_pre.h");
    files.push_back("C:/LSH_test/topolo.h");
    files.push_back("C:/LSH_test/mathexpr_pre.cpp");
    files.push_back("C:/LSH_test/mathexpr.cpp");

    for(int j = 0; j < files.size(); j++)
        files[j] = get_string(files[j]);

    /// [파일] = 비슷한 파일
    vector<set<int>> simfiles;
    simfiles.resize(files.size());

    LSH* lsh = new LSH();
    int b = 20, r = 8;
    vector<vector<lli>> rand_hashes;
    for(int j = 0; j < b * r; j++)
        rand_hashes.push_back(vector<lli>({rand() % INF, rand() % INF}));

    double s = pow(1 / (double)b, 1 / (double)r);
    cout << "s = " << s << endl;

    vector<vector<string>> all_shingles;
    vector<vector<lli>> sig_mat;
    /// LSH
    for(int j = 0; j < files.size(); j++){
        vector<string> shingles = lsh->get_shingles(10, files[j]);
        cout << "get shingles complete " << shingles.size() << endl;
        vector<lli> signature = lsh->get_signature(shingles, rand_hashes);
        cout << "file" << j << " signature: ";
        for(int j = 0; j < signature.size(); j++)
            cout << signature[j] << ", ";
        cout << endl;
        sig_mat.push_back(signature);
        all_shingles.push_back(shingles);
    }
    /// 모든 버켓 출력
    map<string, set<int>> buckets = lsh->run_LSH(sig_mat, b, r);
    for(auto itr = buckets.begin(); itr != buckets.end(); itr++){
        cout << "bucket " << (itr->first) << ": ";
        vector<int> bucket;
        for(auto itr2 = (itr->second).begin(); itr2 != (itr->second).end(); itr2++){
            bucket.push_back(*itr2);
            cout << (*itr2) << ", ";
        }
        /// 한 버킷에 속한 파일 처리
        for(int j = 0; j < bucket.size(); j++){
            for(int i = j + 1; i < bucket.size(); i++){
                simfiles[bucket[j]].insert(bucket[i]);
                simfiles[bucket[i]].insert(bucket[j]);
            }
        }
        cout << endl;
    }

    /// 각 파일 별로 비슷한 파일 출력 (같은 bucket에 한번이라도 존재한 경우)
    cout << "similar files according to LSH (s = " << s << ")" << endl;
    for(int j = 0; j < files.size(); j++){
        cout << j << ": ";
        for(auto itr = simfiles[j].begin(); itr != simfiles[j].end(); itr++)
            cout << (*itr) << ", ";
        cout << endl;
    }

    /// 실제 자카드 유사도 계산
    cout << "actual values" << endl;
    for(int j = 0; j < all_shingles.size(); j++){
        for(int i = j + 1; i < all_shingles.size(); i++){
            set<string> inter, uni;
            double in = 0, un = 0;
            for(int k = 0; k < all_shingles[j].size(); k++){
                inter.insert(all_shingles[j][k]);
                uni.insert(all_shingles[j][k]);
            }
            for(int k = 0; k < all_shingles[i].size(); k++){
                if(inter.find(all_shingles[i][k]) != inter.end())
                    in++;
                uni.insert(all_shingles[i][k]);
            }
            un = uni.size();
            double sim = in / un;
            cout << "sim(" << j << ", " << i << ") = " << sim << endl;
        }
    }

    return 0;
}
