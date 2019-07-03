#ifndef LSH_H
#define LSH_H

#include <string>
#include <vector>
#include <set>
#include <map>
#include <stack>
#define MOD 1000000007
#define ABS(X) ((X) > 0 ? (X) : -(X))
using namespace std;

typedef long long int lli;

class LSH{
private:
    lli string_hashing(string text){
        // polynomial rolling hash function
        lli p = 53, m = MOD;
        lli np = 53;
        lli ret = 0;
        for(int j = 0; j < text.size(); j++, np *= p){
            ret += np * text[j];
            ret %= m;
        }
        return ret;
    }
    lli polynomial_hashing(lli a, lli b, lli x){
        lli p = MOD;
        return (a * ABS(x) + b) % p;
    }
    lli min_hashing(vector<string> shingles, lli a, lli b){
        lli ret = 0; bool ch = false;
        for(int j = 0; j < shingles.size(); j++){
            lli h = this->string_hashing(shingles[j]);
            lli sig = this->polynomial_hashing(a, b, h);
            if(!ch){
                ret = sig;
                ch = true;
            }else if(ret > sig)
                ret = sig;
        }
        return ret;
    }


public:
    LSH(){

    }

    vector<string> get_shingles(int k, string str){
        vector<string> shingles;
        set<string> sg_set;
        for(int j = 0; j < str.size() - k + 1; j++){
            string shingle = "";
            for(int i = 0; i < k; i++)
                shingle += str[j + i];
            sg_set.insert(shingle);
        }
        for(auto itr = sg_set.begin(); itr != sg_set.end(); itr++)
            shingles.push_back(*itr);
        return shingles;
    }
    vector<lli> get_signature(vector<string>& shingles, vector<vector<lli>> rand_hashes){
        vector<lli> signatures;
        for(int j = 0; j < rand_hashes.size(); j++){
            signatures.push_back(this->min_hashing(shingles, rand_hashes[j][0], rand_hashes[j][1]));
        }
        return signatures;
    }
    map<string, set<int>> run_LSH(vector<vector<lli>> sig_mat, lli b, lli r){
        map<string, set<int>> buckets;
        for(int t = 0; t < sig_mat.size(); t++){
            for(int j = 0; j < b; j++){
                string band = "";
                for(int i = j * r; i < (j + 1) * r; i++){
                    lli tmp = sig_mat[t][i];
                    stack<lli> stk;
                    while(tmp){
                        stk.push(tmp % 10);
                        tmp /= 10;
                    }
                    while(!stk.empty()){
                        band += stk.top() + '0';
                        stk.pop();
                    }
                }
                buckets[band].insert(t);
            }
        }
        return buckets;
    }
};

#endif // LSH
