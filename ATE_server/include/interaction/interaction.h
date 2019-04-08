#ifndef ATE_SERVER_INTERACTION_INTERACTION_H_
#define ATE_SERVER_INTERACTION_INTERACTION_H_

#include <string>

/**
  * @brief Interface class for interaction with LVDS board
  **/
class Interaction {
    public:

    /**
     * @brief Get video stream URI from the LVDS board.
     *  It is supposed that the LVDS board is running a video
     *  server that is capable of providing client with such an
     *  identifier. For example, the LVDS board is running a RTSP server. 
     **/
    virtual const std::string & Stream() const = 0;

    /**
     * @brief Taps screen (press + release) at x,y coordinates
     * @param x x coordinate
     * @param y y coordinate
     **/
    virtual void Tap(const int x, const int y) const = 0;

    /**
     * @brief Presses screen at x,y coordinates
     * @param x x coordinate
     * @param y y coordinate
     **/
    virtual void Press(const int x, const int y) const = 0;

    /**
     * @brief Releases screen at x,y coordinates
     * @param x x coordinate
     * @param y y coordinate
     **/
    virtual void Release(const int x, const int y) const = 0;

    /**
     * @brief Drags mouse pointer to x,y coordinates
     * @param x x coordinate
     * @param y y coordinate
     **/
    virtual void Drag(const int x, const int y) const = 0;
};

#endif // ATE_SERVER_INTERACTION_INTERACTION_H_
