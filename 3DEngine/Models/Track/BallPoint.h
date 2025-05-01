#include <vector>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

struct TreeNode {
    PositionIdx pos;
    TreeNode *left;
    TreeNode *right;
};

struct PositionIdx {
    vec2 pos;
    int idx;
};

class BallPoint {

    public:
        BallPoint(vector<vec2> points);

    private:
        void sortTree(vector<PositionIdx> pathPoints, TreeNode &node, bool vertical);
        bool findClose(TreeNode &node, int point1, int point2);
};