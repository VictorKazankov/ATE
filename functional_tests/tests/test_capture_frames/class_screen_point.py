from vhat_client import ScreenPoint as vhat_ScreenPoint

INVALID_RESOLUTION = '0x0'


class ScreenPoint(vhat_ScreenPoint):

    def __init__(self, x, y):
        vhat_ScreenPoint.__init__(self, x, y)

    def __repr__(self):
        return 'ScreenPoint object x:{} y:{}'.format(self.x, self.y)

    def get_resolution_between_other_point(self, point):
        _x = point.x - self.x
        _y = point.y - self.y
        if _x == 0 or _y == 0:
            return INVALID_RESOLUTION
        else:
            return '{}x{}'.format(abs(_x), abs(_y))
