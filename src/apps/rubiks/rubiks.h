#include <graphics/shape.h>
#include <opencv2/opencv.hpp>

struct RubiksCube {
  typedef std::vector<uchar> Plane;
  enum struct Move {
    L, R, T, Bt, F, Bk, H, V
  };
  cv::Mat grid;
  //front facing views
  std::vector<uchar> plane_L = {2,1,0, 11,10,9, 20,19,18};
  std::vector<uchar> plane_R = {6,7,8, 15,16,17, 24,25,26};
  std::vector<uchar> plane_T = {2,5,8, 1,4,7, 0,3,6};
  std::vector<uchar> plane_Bt = {18,21,24, 19,22,25, 20,23,26};
  std::vector<uchar> plane_F = {0,3,6, 9,12,15, 18,21,24};
  std::vector<uchar> plane_Bk = {8,5,2, 17,14,11, 26,23,20};

//public:
  RubiksCube();
  RubiksCube::Plane rotate_plane(Plane plane, bool ckw = true);
  void update_cube(RubiksCube::Plane plane, RubiksCube::Plane new_plane);
};
