#ifndef ATE_SERVER_VIDEO_STREAMING_MATCHING_DETECTOR_H_
#define ATE_SERVER_VIDEO_STREAMING_MATCHING_DETECTOR_H_

#include <string>
#include <opencv2/opencv.hpp>

#include "utils/defines.h"

namespace detector {

/**
  * @brief Interface class for detectors
  **/
class Detector {
    private:
        std::string name_;

    public:

    /**
     * @brief Detect an image pattern at a video frame
     * @param frame video frame received from Sync
     * @param pattern a pattern to be detected
     * @return pattern coordinates
     **/
    virtual const defines::x_y_w_h & Detect(const cv::Mat & frame, const cv::Mat & pattern) const = 0;
 
    /**
     * @brief Gets name of the detector
     * @return name of the detector
     **/
    final const const std::string & Name() const {
        return &name_;
    }
};

}

#endif // ATE_SERVER_VIDEO_STREAMING_MATCHING_DETECTOR_H_
