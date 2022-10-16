#include "listSubsetsOf.h"

void listSubsetsOfIterate(Vector<int> elems) {
    Vector<Vector<int>> subsets = {{}};
    for (int i = 0; i < elems.size(); i++) {
        int size = subsets.size();
        for (int j = 0; j < size; j++)
            subsets.add(subsets[j] + elems[i]);
    }
    for (const auto &subset : subsets)
        std::cout << subset << std::endl;
}

void listSubsetsOf(Vector<int> elems) {
//    listSubsetsOfHelper(elems, {});
    listSubsetsOfHelper(elems, 0, elems.size(), {});
}

void listSubsetsOfHelper(Vector<int> elems, Vector<int> sofar) {
    // 1. Base Case
    if(elems.isEmpty()) {
        std::cout << sofar << std::endl;
        return;
    }
    // 2. Reduce problems
    int first = elems[0];
    Vector<int> remain = elems.subList(1);

    // 3. Recursive Case
    // 3.1 Include
    listSubsetsOfHelper(remain, sofar + first);
    // 3.2 Exclude
    listSubsetsOfHelper(remain, sofar);
}

void listSubsetsOfHelper(Vector<int> &elems, int start, int end, Vector<int> sofar) {
    // 1. Base Case
    if(end - start == 0) {
        std::cout << sofar << std::endl;
        return;
    }
    // 2. Reduce problems

    // 3. Recursive Case
    // 3.1 Include
    listSubsetsOfHelper(elems, start+1, end, sofar + elems[start]);
    // 3.2 Exclude
    listSubsetsOfHelper(elems, start+1, end, sofar);
}
