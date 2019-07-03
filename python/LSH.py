
class LSH:
    def __init__(self):
        pass

    def string_hashing(self, text):
        # polynomial rolling hash function
        p = np = 53
        m = 10 ** 9 + 7
        ret = 0
        for c in text:
            ret += np * ord(c)
            np *= p
            ret %= m
        return ret

    def polynomial_hashing(self, a, b, x):
        p = 10 ** 9 + 7
        return (a * x + b) % p

    def get_shingles(self, k, path):
        file = open(path, 'r', encoding='utf8')
        plain = file.read()
        sg = [plain[i:i + k] for i in range(len(plain) - k + 1)]
        file.close()
        ret = list(set(sg))
        return ret

    def min_hashing(self, shingles, a, b):
        ret, ch = 0, False
        for shingle in shingles:
            h = self.string_hashing(shingle)
            sig = self.polynomial_hashing(a, b, h)
            if not ch:
                ret = sig
                ch = True
            elif ret > sig:
                ret = sig
        return ret

    def get_signature(self, shingles, rand_hashes):
        signatures = []
        for r in rand_hashes:
            signatures.append(self.min_hashing(shingles, r[0], r[1]))
        return signatures

    def run_LSH(self, sig_mat, b, r):
        buckets = dict()
        for t in range(len(sig_mat)):
            sig = sig_mat[t]
            for i in range(b):
                band = ''.join([str(sig[j]) for j in range(i * r, (i + 1) * r)])
                if not band in buckets:
                    buckets[band] = set()
                buckets[band].add(t)
        return buckets

import os
import random

if __name__ == '__main__':
    lsh = LSH()

    b, r = 100, 5
    rand_hashes = [[random.randint(0, 10 ** 9), random.randint(0, 10 ** 9)] for i in range(b * r)]

    s = (1 / b) ** (1 / r)
    print('s =', s)

    path = 'C:/LSH_test/'
    files = os.listdir(path)

    all_shingles = []
    sig_mat = []
    fname = []
    for f in files:
        fp = path + f
        if os.path.isdir(fp):
            continue
        fname.append(f)
        shingles = lsh.get_shingles(10, fp)
        all_shingles.append(set(shingles))
        print('get shingles complete', len(shingles))
        signatures = lsh.get_signature(shingles, rand_hashes)
        print(fp, ':', signatures)
        sig_mat.append(signatures)
    buckets = lsh.run_LSH(sig_mat, b, r)

    simfiles = [set() for i in range(len(fname))]
    # 모든 버킷 출력
    for bucket in buckets:
        print('bucket {}: '.format(bucket), end='')
        tmp_bucket = []
        for fidx in buckets[bucket]:
            tmp_bucket.append(fidx)
            print(fname[fidx], end=', ')
        # 한 버킷에 속한 파일 처리
        for j in range(len(tmp_bucket)):
            for i in range(j + 1, len(tmp_bucket)):
                simfiles[tmp_bucket[j]].add(tmp_bucket[i])
                simfiles[tmp_bucket[i]].add(tmp_bucket[j])
        print()

    # 각 파일 별로 비슷한 파일 출력 (같은 bucket에 한번이라도 존재한 경우)
    print('similar files according to LSH (s = {})'.format(s))
    for j in range(len(fname)):
        print(fname[j] + ': ', end='')
        for f in simfiles[j]:
            print(fname[f] + ', ', end='')
        print()

    print('shingles에 대한 실제 유사도 계산 값')
    for i in range(len(all_shingles)):
        for j in range(i + 1, len(all_shingles)):
            sim = len(all_shingles[i] & all_shingles[j]) / len(all_shingles[i] | all_shingles[j])
            print('sim({}, {}) = {}'.format(fname[i], fname[j], sim))