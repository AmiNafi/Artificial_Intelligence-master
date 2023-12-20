#include <bits/stdc++.h>
using namespace std;
int totalClasstypes, totalAttrTypes, totalIterations, attrValueCnt[6] = {4, 4, 4, 3, 3, 3};
vector < string > buying, maint, doors, persons, lugboot, safety, classVec;
int getID(vector < string > &v, string s) {
	for (int i = 0; i < v.size(); i++) if (v[i] == s) return i + 1;
		return 0;
}
int nodeCnt = 0;
struct mydata {
	int classification;
	vector < int > attributes;
	mydata() {
		attributes = vector < int > (6, 0);
		classification = -1;
	}
};
struct attributeValues {
	vector < int > attributes;
	attributeValues() {
		attributes = vector < int > (6, 0);
	}
};
vector < mydata > all, trainingData, testData;

struct node {
	int isLeaf, leafVal;
	int attributeType, attributeValue;
	struct attributeValues attrVals;
	vector < int > classCnt;
	int totalDerivedClass, prPluraity;
	vector < node > adj;
	node() {
		isLeaf = 0;
		leafVal = attributeValue = attributeType = prPluraity = totalDerivedClass = -1;
		classCnt = vector < int > (totalClasstypes + 1, 0);
	}
};
int getPlurality(struct node *root) {
	pair < int , int > p;
	for (int i = 1; i <= totalClasstypes; i++) {
		if (i == 1) p = make_pair((root->classCnt)[i], i);
		else p = max(p, make_pair((root->classCnt)[i], i));
	}
	return p.second;
}
pair < int, int > getLeafValue(struct node *root) {
	int tot = 0;
	for(int i=0;i<totalAttrTypes;i++){
		if(root->attrVals.attributes[i]) tot++;
	}
	if (tot == totalAttrTypes) {
		int var = 0;
		for (int i = 1; i <= totalClasstypes; i++) {
			var += ((root->classCnt)[i] > 0);
		}
		if (var > 1) {
			return make_pair(1, getPlurality(root));
		}
	}

	if (root->totalDerivedClass == 0) {
		return make_pair(1, root->prPluraity);
	}
	for (int i = 1; i <= totalClasstypes; i++) {
		if (root->totalDerivedClass == root->classCnt[i]) {
			return make_pair(1, i);
		}
	}
	return make_pair(0, -1);
}

double getEntropy(struct node *root) {
	double e = 0, p;
	for (int i = 1; i <= totalClasstypes; i++) {
		p = root->classCnt[i] * 1.0 / root->totalDerivedClass;
		if(p>0) e -= p * log2(p);
	}
	return e;
}
void derivedClassCount(struct node* root) {
	int tot = 0;
	for (int i = 0; i < trainingData.size(); i++) {
		int got = 1;
		for (int j = 0; j < 6; j++) {
			got &= ((root->attrVals).attributes[j] == 0 || trainingData[i].attributes[j] == (root->attrVals).attributes[j]);
		}
		tot += got;
		root->classCnt[trainingData[i].classification] += got;
	}
	root->totalDerivedClass = tot;
}
int getMaxGainAttr(struct node *root) {
	double rootEntropy = getEntropy(root);
	double totChildEntropy = 0, maxInfoGain = -1e16;
	struct attributeValues rtvals = root->attrVals;
	int chosenAttr = 0;
	for (int i = 0; i < totalAttrTypes; i++) {
		if (!rtvals.attributes[i]) { 
			totChildEntropy = 0;
			for (int j = 1; j <= attrValueCnt[i]; j++) {
				root->attrVals.attributes[i] = j;
				totChildEntropy += getEntropy(root);
				root->attrVals.attributes[i] = 0;
			}
			if (rootEntropy - totChildEntropy > maxInfoGain) {
				maxInfoGain = rootEntropy - totChildEntropy;
				chosenAttr = i;
			}
		}
	}
	return chosenAttr;
}
int isOk(struct node *root, mydata mdt) {
	if (root->isLeaf) {
		if (root->leafVal == mdt.classification) return 1;
		return 0;
	}
	for (int i = 0; i < (root->adj).size(); i++) {
		if (mdt.attributes[(root->adj)[i].attributeType] == (root->adj)[i].attributeValue) {
			return isOk(&((root->adj)[i]), mdt);
		}
	}
	return 0;
}

void copyDataToChild(struct node *root, struct node *child){

}

void buildTree(struct node *root) {
	nodeCnt++;
	struct node child;
	child.attributeValue = root->attributeValue;
	child.attrVals = root->attrVals;

	derivedClassCount(root);
	pair < int, int > lf = getLeafValue(root);
	child.isLeaf = root->isLeaf;
	child.leafVal = root->leafVal;

	if (lf.first != 0) {
		root->isLeaf = 1;
		root->leafVal = lf.second;
	}
	else {
		int prPlurality = getPlurality(root);
		child.prPluraity = getPlurality(root);

		int chosenAttr = getMaxGainAttr(root);
		child.attributeType = chosenAttr;
		
		for (int i = 1; i <= attrValueCnt[chosenAttr]; i++) {
			child.attributeValue = i;
			child.attrVals.attributes[chosenAttr] = i;
			(root->adj).push_back(child);
		}
		for (int i = 0; i < (root->adj).size(); i++) {
			buildTree(&root->adj[i]);
		}
	}
}
void init () {
	totalIterations = 20;
	totalClasstypes = 4;
	totalAttrTypes = 6;
	buying = {"vhigh", "high", "med", "low"};
	maint = {"vhigh", "high", "med", "low"};
	doors = {"2", "3", "4", "5more"};
	persons = {"2", "4", "more"};
	lugboot = {"small", "med", "big"};
	safety = {"low", "med", "high"};
	classVec = {"unacc", "acc", "good", "vgood"};
}
void showData(double meanAcc, double maxAcc, double minAcc, double stdDeviation) {
	cout << fixed << setprecision(4);
	cout << "Mean Accuracy: " << meanAcc * 100 << "%" << "\n";
    cout << "Max Accuracy: " << maxAcc * 100 << "%" << "\n";
    cout << "Min Accuracy: " << minAcc * 100 << "%" << "\n";
    cout << "Standard Deviation of Accuracy: " << stdDeviation * 100 << "\n";

}
int main () {
	srand(time(0));
	freopen("car evaluation dataset/car.data", "r", stdin);
	// freopen("car.data", "r", stdin);
	// freopen("out.txt", "w", stdout);
	// cout << " here " << endl;
	init();
	string s, prt;
	int pcnt = 0;
	char delimeter = ',';
	while (cin >> s) {
		// cout << s << endl;
		struct mydata mdt;
		s += delimeter;
		pcnt = 0;
		prt = "";
		for (int i = 0; i < s.length(); i++) {
			if (s[i] != delimeter) prt += s[i];
			else {
				vector < string > v;
				if (pcnt == 0) v = buying;
				else if (pcnt == 1) v = maint;
				else if (pcnt == 2) v = doors;
				else if (pcnt == 3) v = persons;
				else if (pcnt == 4) v = lugboot;
				else if (pcnt == 5) v = safety;
				if (pcnt == 6) {
					mdt.classification = getID(classVec, prt);
				}
				else {
					mdt.attributes[pcnt] = getID(v, prt);
				}

				prt= "";
				pcnt++;
			}
			
		}
		all.push_back(mdt); 
	}
	std::random_device rd;
    std::mt19937 rng(rd());

	vector < double > res;
	int totData = all.size();
	for (int it = 1; it <= totalIterations; it++) {
		shuffle(all.begin(), all.end(), rng);

		for (int i = 0; i < totData * 0.2; i++) {
			testData.push_back(all[i]);
		}
		for (int i = totData * 0.2 ; i < totData; i++) {
			trainingData.push_back(all[i]);
		}
		
		struct node root;
		buildTree(&root);
		int totOk = 0;
		for (int i = 0; i < testData.size(); i++) {
			int ok = isOk(&root, testData[i]);
			if (ok) totOk++;
		}
		double acc = 1.0 * totOk / testData.size();
		res.push_back(acc);

		trainingData.clear();
		testData.clear();
	}
	sort(res.begin(), res.end());
	double minAcc = res[0], meanAcc = 0, stdDeviation = 0, maxAcc = res.back();
	for (int i = 0; i < res.size(); i++) {
		meanAcc += res[i];
	}
	meanAcc = meanAcc / res.size();
	for (int i = 0; i < res.size(); i++) {
		stdDeviation += (meanAcc - res[i]) * (meanAcc - res[i]);
	}
	stdDeviation /= res.size();
	stdDeviation = sqrt(stdDeviation);
	showData(meanAcc, maxAcc, minAcc, stdDeviation);
}