#include <iostream>
#include <string>

#include "console.h"
#include "vector.h"
using namespace std;

#define btr

#ifdef btr
void listSubsetsRec(const Vector<int> &elems, Vector<int> &soFar, int index, Vector<Vector<int>> &result) {
    result.add(soFar);  // base case已经包含在for条件里了
    //（此版本的不选择，其实就隐含在for循环里，跳到下一个元素，以下一个元素为基准开始）
    for(int i=index; i<elems.size(); ++i) {
        soFar.add(elems[i]);  // 添加当前元素
        listSubsetsRec(elems, soFar, i+1, result);  // 只在添加该元素基础上探索
        soFar.remove(soFar.size() - 1);  // 恢复原状态
    }
}
#endif

#ifdef dfs
// 递归回溯：DFS
void listSubsetsRec(const Vector<int> &elems, Vector<int> &soFar, int index, Vector<Vector<int>> &result) {
    if (index == elems.size()) {
        result.add(soFar);
    } else {
        soFar.add(elems[index]);
        listSubsetsRec(elems, soFar, index + 1, result);
        soFar.remove(soFar.size() - 1);
        listSubsetsRec(elems, soFar, index + 1, result);
    }
}
#endif

void listSubsetsOf(const Vector<int> &elems) {
    Vector<int> soFar = {};
    Vector<Vector<int>> result;
    listSubsetsRec(elems, soFar, 0, result);
    for (const Vector<int> &item : result)
        cout << item << endl;
}

int main() {
    Vector<int> elems = {1, 3, 5};
    listSubsetsOf(elems);
    return 0;
}
