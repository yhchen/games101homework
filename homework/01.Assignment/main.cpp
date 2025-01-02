#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;
#define DEG2RAD(_DEG) ((_DEG) / 180 * MY_PI)

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1, -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.

    double rad = DEG2RAD(rotation_angle);
    auto c = cos(rad);
    auto s = sin(rad);
    model << c, -s, 0, 0, // x
        s, c, 0, 0,       // y
        0, 0, 1, 0,       // z
        0, 0, 0, 1;       // w

    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio, float zNear, float zFar)
{
    // Students will implement this function

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    float eyeAngle = DEG2RAD(eye_fov / 2);
    float top = zNear * std::tan(eyeAngle);
    float bottom = -top;
    float right = top * aspect_ratio;
    float left = -right;

    // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.
    Eigen::Matrix4f moveMat = Eigen::Matrix4f::Identity();
    moveMat << 1, 0, 0, -(right + left) / 2, // x
        0, 1, 0, -(top + bottom) / 2,        // y
        0, 0, 1, -(zNear + zFar) / 2,        // z
        0, 0, 0, 1;                          // w

    Eigen::Matrix4f scaleMat(4, 4);
    scaleMat << 2 / (right - left), 0, 0, 0, // x
        0, 2 / (top - bottom), 0, 0,         // y
        0, 0, 2 / (zNear - zFar), 0,         // z
        0, 0, 0, 1;                          // w

    Eigen::Matrix4f persp2OrthoMat(4, 4);
    persp2OrthoMat << zNear, 0, 0, 0,      // x
        0, zNear, 0, 0,                    // y
        0, 0, zNear + zFar, -zNear * zFar, // z
        0, 0, 1, 0;                        // w

    // 为了使得三角形是正着显示的，这里需要把透视矩阵乘以下面这样的矩阵
    // 参考：http://games-cn.org/forums/topic/%e4%bd%9c%e4%b8%9a%e4%b8%89%e7%9a%84%e7%89%9b%e5%80%92%e8%bf%87%e6%9d%a5%e4%ba%86/
    Eigen::Matrix4f Mt(4, 4);
    Mt << 1, 0, 0, 0, // x
        0, 1, 0, 0,   // y
        0, 0, -1, 0,  // z
        0, 0, 0, 1;   // w

    persp2OrthoMat = persp2OrthoMat * Mt;

    projection = scaleMat * moveMat * persp2OrthoMat * projection;
    return projection;
}

int main(int argc, const char **argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3)
    {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4)
        {
            filename = std::string(argv[3]);
        }
        else
            return 0;
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line)
    {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27)
    {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a')
        {
            angle += 10;
        }
        else if (key == 'd')
        {
            angle -= 10;
        }
    }

    return 0;
}
