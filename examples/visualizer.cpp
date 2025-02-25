#include <iostream>

#include <cilantro/utilities/point_cloud.hpp>
#include <cilantro/visualization/common_renderables.hpp>
#include <cilantro/visualization/visualizer.hpp>

void callback(cilantro::Visualizer& viz, const std::string& name) {
  std::cout << "Toggling visibility for " << name << std::endl;
  viz.toggleVisibility(name);
}

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cout << "Please provide path to PLY file." << std::endl;
    return 0;
  }

  cilantro::PointCloud3f cloud(argv[1]);

  // First
  const std::string window_name1 = "Visualizer demo (window 1)";
  pangolin::CreateWindowAndBind(window_name1, 640, 480);
  cilantro::Visualizer viz1(window_name1, "disp1");

  std::vector<float> scalars(cloud.size());
  for (size_t i = 0; i < cloud.size(); i++) {
    scalars[i] = cloud.points.col(i).norm();
  }

  viz1.addObject<cilantro::PointCloudRenderable>("pcd", cloud,
                                                 cilantro::RenderingProperties()
                                                     .setColormapType(cilantro::ColormapType::JET)
                                                     .setLineDensityFraction(0.2f)
                                                     .setUseLighting(false))
      ->setPointValues(scalars);
  viz1.addObject<cilantro::CoordinateFrameRenderable>(
      "axis", Eigen::Matrix4f::Identity(), 0.4f,
      cilantro::RenderingProperties().setLineWidth(5.0f));
  viz1.addObject<cilantro::TextRenderable>("text", "Coordinate Frame", 0, 0, 0,
                                           cilantro::RenderingProperties()
                                               .setFontSize(20.0f)
                                               .setPointColor(1.0f, 1.0f, 0.0f)
                                               .setTextAnchorPoint(0.5f, -1.0f));

  // Second
  cilantro::PointCloud3f cloud2(cloud);
  for (size_t i = 0; i < cloud2.size(); i++) {
    cloud2.points.col(i) += Eigen::Vector3f(1.0, 0.0, 1.0);
  }

  const std::string window_name2 = "Visualizer demo (window 2)";
  pangolin::CreateWindowAndBind(window_name2, 640, 480);
  cilantro::Visualizer viz2(window_name2, "disp2");

  viz2.addObject<cilantro::PointCloudRenderable>(
      "pcd1", cloud,
      cilantro::RenderingProperties().setPointColor(1.0f, 0.0f, 0.0f).setOpacity(0.4f));
  viz2.addObject<cilantro::PointCloudRenderable>(
      "pcd2", cloud2,
      cilantro::RenderingProperties().setPointColor(0.0f, 0.0f, 1.0f).setOpacity(0.4f));
  viz2.addObject<cilantro::PointCorrespondencesRenderable>(
      "correspondences", cloud2, cloud,
      cilantro::RenderingProperties().setLineDensityFraction(0.005).setOpacity(0.3f));
  viz2.addObject<cilantro::CoordinateFrameRenderable>(
      "axis", Eigen::Matrix4f::Identity(), 0.4f,
      cilantro::RenderingProperties().setLineWidth(5.0f));

  viz2.registerKeyboardCallback('c', std::bind(callback, std::ref(viz2), "correspondences"));

  std::cout << "Press 'n' to toggle rendering of normals" << std::endl;
  while (!viz1.wasStopped() && !viz2.wasStopped()) {
    viz1.spinOnce();
    viz2.spinOnce();
  }

  return 0;
}
