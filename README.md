# LSH
LSH: Locality Sensitive Hashing 구현

## 1. Python
### 1.1. Shingling
  다음과 같이 shingling을 하여 file_path파일의 k-shingles를 추출할 수 있다. 이때, k-shingles는 리스트로 반환되며 중복되는 shingle은 카운팅 하지 않는다.
```python
lsh = LSH()
shingles = lsh.get_shingles(k, file_path)
```

### 1.2. Min-Hashing
  min-hashing은 h(x) = (ax + b) % p와 같은 polynomial hash function을 이용한다. 따라서 signature를 구하기 위해 \[a, b\]값으로 구성된 리스트를 구성해야 한다. 해당 리스트의 크기는 구할 signature의 크기와 동일하다. 다음과 같이 수행하면 signatures에는 signature를 담고 있는 리스트가 저장된다.
```python
rand_hashes = [[random.randint(0, 10 ** 9), random.randint(0, 10 ** 9)] for i in range(b * r)] # b, r은 각각 밴드 수, 한 밴드에 속한 row 수
signatures = lsh.get_signature(shingles, rand_hashes)
```

### 1.3. Locality Sensitive Hashing
  LSH를 수행하기 위해 먼저 Signature Matrix를 구성해야 한다. files가 LSH를 수행할 파일의 경로들을 담고 있는 리스트라면, 다음과 같이 Signature Matrix인 sig_mat을 구성한다.
```python
sig_mat = []
for f in files:
    shingles = lsh.get_shingles(k, f)
    signatures = lsh.get_signature(shingles, rand_hashes)
    sig_mat.append(signatures)
```

  구한 Signature Matrix를 바탕으로 다음과 같이 LSH를 수행한다. buckets에는 각 band값을 해시화한 값 즉, bucket의 키값을 키로 가지고 해당 bucket에 속한 원소를 저장하는 집합(set)을 값으로 가지는 딕셔너리(dict)가 저장된다.
```python
buckets = lsh.run_LSH(sig_mat, b, r)
```

## 2. C++
  아래서 부터 등장하는 `lli`는 `long long int` 타입을 뜻한다.

### 1.1. Shingling
  다음과 같이 shingling을 하여 str문자열에서 k-shingles를 추출할 수 있다. 이때, k-shingles는 `vector<string>`으로 반환되며 중복되는 shingle은 카운팅 하지 않는다.
```cpp
LSH* lsh = new LSH();
vector<string> shingles = lsh->get_shingles(k, str);
```

### 1.2. Min-Hashing
  min-hashing은 h(x) = (ax + b) % p와 같은 polynomial hash function을 이용한다. 따라서 signature를 구하기 위해 {a, b}값으로 구성된 배열을 구성해야 한다. 해당 배열의 크기는 구할 signature의 크기와 동일하다. 다음과 같이 수행하면 signatures에는 signature를 담고 있는 `vector<lli>`가 저장된다.
```python
vector<vector<lli>> rand_hashes;
for(int j = 0; j < b * r; j++)
    rand_hashes.push_back(vector<lli>({rand() % INF, rand() % INF})); // b, r은 각각 밴드 수, 한 밴드에 속한 row 수
vector<lli> signature = lsh->get_signature(shingles, rand_hashes);
```

### 1.3. Locality Sensitive Hashing
  LSH를 수행하기 위해 먼저 Signature Matrix를 구성해야 한다. files가 LSH를 수행할 파일의 내용을 담고 있는 배열이라면, 다음과 같이 Signature Matrix인 sig_mat을 구성한다.
```python
vector<vector<lli>> sig_mat;
for(int j = 0; j < files.size(); j++){
    vector<string> shingles = lsh->get_shingles(k, files[j]);
    vector<lli> signature = lsh->get_signature(shingles, rand_hashes);
    sig_mat.push_back(signature);
}
```

  구한 Signature Matrix를 바탕으로 다음과 같이 LSH를 수행한다. buckets에는 각 band값을 해시화한 값 즉, bucket의 키값을 키로 가지고 해당 bucket에 속한 원소를 저장하는 집합(set)을 값으로 가지는 딕셔너리(map)가 저장된다.
```python
map<string, set<int>> buckets = lsh->run_LSH(sig_mat, b, r);
```
