import logging

import allure
from functional_tests.pages.hmi import capture_frames as hmi_capture_frames
from functional_tests.tests.test_capture_frames.class_screen_point import (
    INVALID_RESOLUTION, ScreenPoint)
from functional_tests.utils import ssh_commands as cmd
from functional_tests.utils.ssh_connect import (
    execute_command, execute_command_and_get_result, start)

DIR_PREFIX = '/var/lib/vdp/vhat'
DEF_FOLDER_NAME = 'tmp_capture_frames'
DEF_INTERVAL = 10
DEF_DURATION = 100
DEF_LEFT_POINT = ScreenPoint(0, 0)
DEF_RIGHT_POINT = ScreenPoint(60, 60)

MIN_LEFT_POINT = ScreenPoint(0, 0)
MIN_INTERVAL = 0
MAX_DURATION = 5000

usage_exc_message = 'Please perform do_capture_frames 1st'


class UsageError(Exception):
    """ User Exception
    Warn user in case of calling function and situation where no data for it to perform
    It means additional actions should be performed before call function
    """
    pass


class CaptureFramesTest(object):

    def __init__(self, dir_name):
        self.ssh_client = start()
        self.dir_name = dir_name
        self.path_dir = r"{}/{}".format(DIR_PREFIX, self.dir_name)

        self.result_of_capture_frames = None
        self.left_point = None
        self.right_point = None
        self.expected_resolution = None

        self.interval = None
        self.duration = None

    def __del__(self):
        self.ssh_client.close()

    @property
    def result_of_capture_frames(self):
        if not self._result_of_capture_frames:
            raise UsageError(usage_exc_message)
        return self._result_of_capture_frames

    @result_of_capture_frames.setter
    def result_of_capture_frames(self, value):
        self._result_of_capture_frames = value

    @property
    def expected_resolution(self):
        if not self._expected_resolution:
            raise UsageError(usage_exc_message)
        return self._expected_resolution

    @expected_resolution.setter
    def expected_resolution(self, value):
        if value == INVALID_RESOLUTION:
            self._expected_resolution = self._get_current_screen_resolution()
        else:
            self._expected_resolution = value

    @allure.step("Perform call for API captureFrames")
    def do_capture_frames(self,
                          interval=DEF_INTERVAL,
                          duration=DEF_DURATION,
                          top_left_coordinate=DEF_LEFT_POINT,
                          bottom_right_coordinate=DEF_RIGHT_POINT):

        self.interval = interval
        self.duration = duration

        self.left_point = top_left_coordinate
        self.right_point = bottom_right_coordinate
        self.expected_resolution = self.left_point.get_resolution_between_other_point(self.right_point)

        self.result_of_capture_frames = hmi_capture_frames(interval,
                                                           duration,
                                                           top_left_coordinate,
                                                           bottom_right_coordinate,
                                                           self.dir_name)
        logging.info('result of API is [{}]'.format(self.result_of_capture_frames))

        return self

    def delete_dir(self):
        if self.dir_name == '' or self.path_dir.rstrip('/') == DIR_PREFIX:
            logging.warning('Trying to remove init folder! SKIPPED')
        else:
            execute_command(self.ssh_client, cmd.SUDO_RM.format(self.path_dir), True, ignore_error=True)

    @allure.step("Check image creation at file system")
    def check_that_files_exist(self):
        size = None
        for image_file in self.result_of_capture_frames:
            path = r"{}/{}".format(self.path_dir, image_file)
            output = execute_command_and_get_result(self.ssh_client, r"ls '{}'".format(path))
            # check that file exists
            assert output == path
            size_command = 'stat -c %s "{}"'.format(path)
            size = int(execute_command_and_get_result(self.ssh_client, size_command))
            # check that is not empty
            assert size > 0

        logging.info('actual picture size is [{}] dir is [{}]'.format(size, self.dir_name))
        return self

    @allure.step("Check that one frame is created")
    def check_that_frame_is_one(self):
        assert 1 == len(self.result_of_capture_frames)
        return self

    @allure.step("Check that several frames are created")
    def check_that_frame_is_not_one(self):
        assert 1 < len(self.result_of_capture_frames)
        return self

    @allure.step("Check resolution for image(s)")
    def check_picture_resolution(self):
        picture_resolution = None
        for image_file in self.result_of_capture_frames:
            path = r"{}/{}".format(self.path_dir, image_file)
            output = execute_command(self.ssh_client, 'identify "{}"'.format(path)).read().strip()
            picture_data = output[output.lower().index(').png'):]
            picture_resolution = picture_data.split(' ')[2]
            # check resolution of picture
            assert picture_resolution == self.expected_resolution

        logging.info('actual picture resolution is [{}]'.format(picture_resolution))
        return self

    @allure.step("Getting current performance for API")
    def get_avg_frames(self):
        if not self.result_of_capture_frames:
            raise UsageError(usage_exc_message)

        res = float(len(self.result_of_capture_frames)) / self.duration * 1000

        logging.info('current performance is {:.2f} frames per sec '
                     'during {} ms with interval {} ms'.format(res, self.duration, self.interval))
        return self

    def get_current_width_and_height(self):
        def get_result_from_output(string):
            return int(string.strip().split('=')[1].strip())

        width = execute_command_and_get_result(self.ssh_client, cmd.GET_CURRENT_WIDTH)
        height = execute_command_and_get_result(self.ssh_client, cmd.GET_CURRENT_HEIGHT)
        res = (get_result_from_output(width), get_result_from_output(height))
        return res

    def _get_current_screen_resolution(self):
        return '{}x{}'.format(*self.get_current_width_and_height())
