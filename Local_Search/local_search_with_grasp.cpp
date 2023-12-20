
#include <bits/stdc++.h>
#define pii pair < int, int >
#define info pair < int, pii > 	
#define endl "\n"
using namespace std;
int maxLocalIteration = 10000, total_nodes, total_edges, totfiles = 54, itCnt = 20;
const int M = 5003;
int setof[M];
vector < pii > adj[M];
vector < info > all;

void partition(double alpha) {
	int minw = (all.begin())->first;
	int maxw = (all.rbegin())->first;
	int miu = minw + round(alpha * (maxw - minw));
	int id = lower_bound(all.begin(), all.end(), make_pair(miu, pii(0, 0))) - all.begin();
	int ln = total_edges - id;
	id += rand() % ln;
	info ii = all[id];
	setof[ii.second.first] = 0;
	setof[ii.second.second] = 1;
	vector < int > unused;
	for (int i = 1; i <= total_nodes; i++) {
		if (setof[i] == -1) unused.push_back(i);
	}
	while (unused.size()) {
		minw = INT_MAX;
		maxw = INT_MIN;
		int cursz = unused.size();
		vector < pii > ara(cursz, pii(minw, 0));
		for (int i = 0; i < cursz; i++) {
			ara[i].second = i;
		}
		vector < int > score1(cursz, 0), score2(cursz, 0);
		for (int i = 0; i < cursz; i++) {
			int u = unused[i];
			for (pii p : adj[u]) {
				int v = p.first, w = p.second;
				if (setof[v] != -1) {
					if (setof[v] == 0) score2[i] += w;
					else score1[i] += w;
					ara[i].first = max(score1[i], score2[i]);
					minw = min(minw, min(score1[i], score2[i]));
				}
			}
		}
		sort(ara.begin(), ara.end());
		maxw = (ara.rbegin())->first;
		miu = minw + round(alpha * (maxw - minw));
		id = lower_bound(ara.begin(), ara.end(), pii(miu, 0)) - ara.begin();
		ln = cursz - id;
		id += rand() % ln;
		int relid = ara[id].second;
		int myid = unused[relid];
		if (score1[relid] > score2[relid]) setof[myid] = 0;
		else setof[myid] = 1;
		unused.erase(unused.begin() + relid);
	}
}
void rand_partition() {
	mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
	vector<int> permutation(total_nodes);
	for (int i = 0; i < total_nodes; i++)
        permutation[i] = i;
    shuffle(permutation.begin(), permutation.end(), rng);
    for (int i = 0; i < total_nodes; i++) {
    	setof[permutation[i] + 1] = (i&1);
    }
    

}
int localSearch() {
	int change = true, curLocalIteration = 0;
	while (change) {
		if (curLocalIteration == maxLocalIteration) break;
		curLocalIteration++;
		//cerr << curLocalIteration << endl;
		change = false;
		for (int v = 1, score1, score2; v <= total_nodes; v++) {
			score1 = score2 = 0;
			for (pii pu : adj[v]) {
				int u = pu.first, w = pu.second;
				if (setof[u] == setof[v]) score1 += w;
				else score2 += w;
			}
			//cerr << v << " scores " << score1 << " " << score2 << endl;
			if (score1 > score2) {
				setof[v] ^= 1;
				change = true;
				break;
			} 
		}
	}
	return curLocalIteration;
}
int getAns() {
	int ret = 0;
	for (info ii : all) {
		int u = ii.second.first, v = ii.second.second, w = ii.first;
		if (setof[u] != setof[v]) ret += w; 
	}
	return ret;
}
info grasp(int maxiterations) {
	int localIt = 0, localAns = 0;
	random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<> genrand(0.5, 1.0);
    int ans = INT_MIN;
	for (int i = 1; i <= maxiterations; i++) {
		double alpha = genrand(mt);
		for (int j = 0; j <= total_nodes; j++) setof[j] = -1;
		partition(alpha);
		localIt += localSearch();
		int cur = getAns();
		localAns += cur;
		ans = max(ans, cur);
	}
	int avgIt = localIt / maxiterations;
	int avgAns = localAns / maxiterations;
	return make_pair(ans, pii(avgAns, avgIt));
}
int semiGreedyVal(int totalIteration) {
	random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<> genrand(0.5, 1.0);
	int ans = 0;
	for (int i = 1; i <= totalIteration; i++) {
		for (int j = 0; j <= total_nodes; j++) setof[j] = -1;
		double alpha = genrand(mt);
		partition(alpha);
		partition(1.0);
		ans += getAns();
	}
	return ans / totalIteration;
}
int greedyVal(int totalIteration) {
	int ans = 0;
	for (int i = 1; i <= totalIteration; i++) {
		for (int j = 0; j <= total_nodes; j++) setof[j] = -1;
		double alpha = 1.0;
		partition(alpha);
		ans += getAns();
	}
	return ans / totalIteration;
}
int randomizedVal(int totalIteration) {
	int ans = 0;
	for (int i = 1; i <= totalIteration; i++) {
		for (int j = 0; j <= total_nodes; j++) setof[j] = -1;
		rand_partition();
		ans += getAns();
	}
	return ans / totalIteration;
}

int bestVal[51];
int main () {
	freopen("out.txt", "w", stdout);
	bestVal[1] = 12078;
    bestVal[2] = 12084;
    bestVal[3] = 12077;
    bestVal[11] = 627;
    bestVal[12] = 621;
    bestVal[13] = 645;
    bestVal[14] = 3187;
    bestVal[15] = 3169;
    bestVal[16] = 3172;
    bestVal[22] = 14123;
    bestVal[23] = 14129;
    bestVal[24] = 14131;
    bestVal[32] = 1560;
    bestVal[33] = 1537;
    bestVal[34] = 1541;
    bestVal[35] = 8000;
    bestVal[36] = 7996;
    bestVal[37] = 8009;
    bestVal[43] = 7027;
    bestVal[44] = 7022;
    bestVal[45] = 7020;
    bestVal[48] = 6000;
    bestVal[49] = 6000;
    bestVal[50] = 5988;

    cout << "Problem,,,,Constructive algorithm,,,Local Search,,Grasp,,Known best solution\nName,No. of vertex, No. of edges, Weight range, Randomized, Greedy, Semi greedy, Simple local,, Grasp\n,,,,,,,No. of iteration, Best value,No. of iteration, Best value\n";
    string fname = "";
    for (int i = 1; i <= totfiles; i++) {
    	fname = "set1/g" + to_string(i) + ".rud";
    	//cerr << fname << " here " << endl;
    	freopen((char*)(&fname[0]), "r", stdin);
    	srand(time(0));
    	cin >> total_nodes >> total_edges;
    	//cerr << total_nodes << " and " << total_edges << endl;
    	int ans = 0;
    	for (int i = 0, u, v, w; i < total_edges; i++) {
    		cin >> u >> v >> w;
    		all.push_back(make_pair(w, pii(u, v)));
    		adj[u].push_back(pii(v, w));
    		adj[v].push_back(pii(u, w));
    	}
    	sort(all.begin(), all.end());
    	int randAns = randomizedVal(itCnt);
    	int greedyAns = greedyVal(itCnt);
    	int semiGreedyAns = semiGreedyVal(itCnt);
    	info ii = grasp(itCnt);
    	int graspAns = ii.first;
    	int localAns = ii.second.first;
    	int localIt = ii.second.second;
    	string bst = to_string(bestVal[i]);
    	if (bestVal[i] == 0) bst = "Unknown";
    	cout << "G" << i << "," << total_nodes << "," << total_edges << "," << "[ " << ((all.begin())->first) << " to " << ((all.rbegin())->first) << " ]" << "," << randAns << "," << greedyAns << "," << semiGreedyAns << "," << localIt << "," << localAns << "," << itCnt << "," << graspAns << "," << bst << "\n";
    }
}