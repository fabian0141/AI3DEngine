#include "BallPoint.h"
#include <algorithm>  

BallPoint::BallPoint(const vector<vec2>& points) {
    for (int i = 0; i < points.size(); i++) {
        positions.push_back(PositionIdx{points[i], i});
    }

    root = new TreeNode{};
    sortTree(positions, root, true);
}

BallPoint::~BallPoint() {
    std::function<void(TreeNode*)> deleteTree = [&](TreeNode* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    };
    deleteTree(root);
}

void BallPoint::sortTree(vector<PositionIdx>& pathPoints, TreeNode* node, bool vertical) {
    if (pathPoints.empty()) {
        node = nullptr;
        return;
    }

    int middle = pathPoints.size() / 2;

    // Partition vector so that middle is the median element
    nth_element(pathPoints.begin(), pathPoints.begin() + middle, pathPoints.end(), [vertical](const PositionIdx& left, const PositionIdx& right) {
        return vertical ? (left.pos.x < right.pos.x) : (left.pos.y < right.pos.y);
    });

    node->pos = pathPoints[middle];

    vector<PositionIdx> leftPoints(pathPoints.begin(), pathPoints.begin() + middle);
    vector<PositionIdx> rightPoints(pathPoints.begin() + middle + 1, pathPoints.end());

    if (!leftPoints.empty()) {
        node->left = new TreeNode{};
        sortTree(leftPoints, node->left, !vertical);
    }
    if (!rightPoints.empty()) {
        node->right = new TreeNode{};
        sortTree(rightPoints, node->right, !vertical);
    }
}

bool BallPoint::findClose(int point1, int point2) {
    return findClose(root, point1, point2);
}

bool BallPoint::findClose(TreeNode* node, int point1, int point2) {
    if (!node) return false;

    return false;
}
