#include <bits/stdc++.h>
using namespace std;
#define Matrix vector < vector  < int > >
#define pii pair < int, int >
#define vi vector < int >
int fx[4] = {0, 0, 1, -1};
int fy[4] = {1, -1, 0, 0};
set < pair<Matrix, int> > st;
map < int, Matrix > mp;
vector < int > parent, dis, vis;
int invCount(const Matrix &all) {
	int n = all.size();
	int ret = 0;
	vi v;
	for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) {
		if (all[i][j]) v.push_back(all[i][j]);
	}
	for (int i = 0; i < v.size(); i++) {
		for (int j = i + 1; j < v.size(); j++) {
			ret += v[i] > v[j];
		}
	}
	return ret;
}
pii getzero(const Matrix &mat) {
	int n = mat.size();
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (mat[i][j] == 0) return pii(i, j);
		}
	}
	return pii(0, 0);
}
bool canSolve(const Matrix &all) {
	int icnt = invCount(all);
	int n = all.size();
	// cout << n << " and " << icnt << endl;
	if (n % 2 && icnt % 2) return 0;
	if (n % 2 == 0 && (icnt + n - 1 - getzero(all).first) % 2) return 0;
	return 1;
}
bool operator<(const pair <Matrix, int> &a, const pair <Matrix, int> &b) {
	return a.first < b.first;
	// return 0;
}

// int setid(Matrix &mat) {
	
// }
int man_dis(const Matrix &mat) {
	int n = mat.size();
	int ret = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int val = mat[i][j];
			if (val == 0) val = n * n;
			int ni = (val - 1) / n;
			int nj = (val - 1) % n;
			ret += max(ni - i, i - ni) + max(nj - j, j - nj);
		}
	}
	return ret;
}
int ham_dis(const Matrix &mat) {
	int n = mat.size();
	int ret = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int val = mat[i][j];
			if (val == 0) val = n * n;
			ret += (val != i * n + j + 1);
		}
	}
	return ret;
}
int expanded = 0, explored = 0;
int getdis(int id, int t) {
	if (t == 0) return ham_dis(mp[id]);
	else return ham_dis(mp[id]);
}
int getid (Matrix &mat) {
	auto it = st.lower_bound(make_pair(mat, 0));
	if (it->first == mat) return it->second;
	st.insert(make_pair(mat, (int)(st.size())));
	mp[st.size() - 1] = mat;
	dis.push_back(1e9);
	vis.push_back(0);
	parent.push_back(0);
	explored++;
	return st.size() - 1;
}
int dijk(Matrix &mat, int tp) {
	priority_queue < pii, vector < pii >, greater <pii> > pq;
	int s = getid(mat);
	int d = getdis(s, tp);
	pq.push(pii(d, s));
	dis[s] = 0;
	parent[s] = s;
	while (pq.size()) {
		pii pu = pq.top();
		pq.pop();
		int u = pu.second;
		// cout << u << " Here " << dis[u] << endl;
		// for (int i = 0; i < mp[u].size(); i++) {
		// 	for (int j = 0; j < mp[u].size(); j++) {
		// 		cout << mp[u][i][j] << " ";
		// 	}
		// 	cout << endl;
		// }
		if (getdis(u, tp) == 0) return u;
		if (vis[u]) continue;
		vis[u] = 1;
		pii zeropos = getzero(mp[u]);
		int sz = mp[u].size();
		Matrix mat = mp[u];
		int got = 0;
		for (int i = 0; i < 4; i++) {
			int ni = zeropos.first + fx[i];
			int nj = zeropos.second + fy[i];
			if (ni >= 0 && ni < sz && nj >= 0 && nj < sz) {
				swap(mat[zeropos.first][zeropos.second], mat[ni][nj]);
				int curid = getid(mat);
				if (dis[curid] > dis[u] + 1) {
					pq.push(pii(dis[u] + 1 + getdis(curid, tp), curid));
					dis[curid] = dis[u] + 1;
					parent[curid] = u;
					got = 1;
				}
				swap(mat[zeropos.first][zeropos.second], mat[ni][nj]);
			}
		}
		expanded += got;
	}
	return 0;
}
int main() {
	int n;
	cin >> n;
	Matrix mat(n, vi(n));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> mat[i][j];
		}
	}
	if (canSolve(mat) == 0) cout << "Not Solvable" << endl;
	else {
		vector < int > all;
		// cout << " here now this " << endl;
		int cur = dijk(mat, 0);
		int ans = dis[cur];
		
		while (1) {
			all.push_back(cur);
			if (cur == 0) break;
			cur = parent[cur];
		} 
		cout << "Solvable with minimum Moves = " << ans << endl;
		reverse(all.begin(), all.end());
		for (int x : all) {
			for (int i = 0; i < mp[x].size(); i++) {
				for (int j = 0; j < mp[x].size(); j++) {
					if (mp[x][i][j] == 0) cout << "* ";
					else cout << mp[x][i][j] << " ";
				}
				cout << endl;
			}
			cout << endl;
		}
		cout << "For manhattan distance explored = " << explored << " and expanded = " << expanded << endl;
		st.clear();
		dis.clear();
		vis.clear();
		parent.clear();
		mp.clear();
		expanded = explored = 0;
		// dijk(mat, 1);
		cout << "For hamming distance explored = " << explored << " and expanded = " << expanded << endl;
	}
	return 0;
}