#include "BallPoint.h"
#include <algorithm>  

BallPoint::BallPoint(vector<vec2> points) {
    TreeNode root = {};
    vector<PositionIdx> positions;
    for (int i = 0; i < points.size(); i++)
    {
        positions.push_back(PositionIdx{points[i], i});
    }
    
    sortTree(positions, root, true);
}

void BallPoint::sortTree(vector<PositionIdx> pathPoints, TreeNode &node, bool vertical) {

    int middle = pathPoints.size() / 2;
    
    nth_element(pathPoints.begin(), pathPoints.begin() + middle, pathPoints.end(), [=](PositionIdx left, PositionIdx right){
        if (vertical) {
            return left.pos.x < right.pos.x;
        } else {
            return left.pos.y < right.pos.y;
        }
    });

    vector<PositionIdx> leftPoints;
    vector<PositionIdx> rightPoints;
    copy(pathPoints.begin(), pathPoints.begin() + middle, leftPoints);
    copy(pathPoints.begin() + middle + 1, pathPoints.end(), rightPoints);

    node.left = new TreeNode{};
    node.right = new TreeNode{};
    sortTree(leftPoints, *node.left, !vertical);
    sortTree(leftPoints, *node.right, !vertical);
}

bool BallPoint::findClose(TreeNode &node, int point1, int point2) {
    
}
