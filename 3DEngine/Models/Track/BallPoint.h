#include <vector>
#include <glm/glm.hpp>
#include <algorithm>

using namespace glm;
using namespace std;

struct PositionIdx {
    vec2 pos;
    int idx;
};

struct TreeNode {
    PositionIdx pos;
    TreeNode *left = nullptr;
    TreeNode *right = nullptr;
};

class BallPoint {
public:
    BallPoint(const vector<vec2>& points);
    ~BallPoint();

    bool findClose(int point1, int point2);  // Searches for closeness in tree

private:
    void sortTree(vector<PositionIdx>& pathPoints, TreeNode* node, bool vertical);
    bool findClose(TreeNode* node, int point1, int point2);

    TreeNode* root = nullptr;
    vector<PositionIdx> positions;
};