import logging

import pytest
from functional_tests.pages.hmi import capture_frames as hmi_capture_frames
from functional_tests.tests.test_capture_frames.class_screen_point import \
    ScreenPoint
from functional_tests.tests.test_capture_frames.class_test_capture_frames import (
    DEF_DURATION, DEF_FOLDER_NAME, DEF_INTERVAL, DEF_LEFT_POINT,
    DEF_RIGHT_POINT, MAX_DURATION, MIN_INTERVAL, MIN_LEFT_POINT,
    CaptureFramesTest)
from functional_tests.utils import ssh_commands as cmd
from functional_tests.utils.report import jira_test
from functional_tests.utils.ssh_connect import (
    execute_command, execute_command_and_get_result, execute_list_of_commands,
    start)
from vhat_client import (InvalidDuration, InvalidRectangleCoordinates,
                         NoAvailableDiskSpace, PermissionDenied,
                         VideoStreamingError)
from vhat_client import captureFrames as vhat_captureFrames

pytestmark = [pytest.mark.captureFrames, pytest.mark.regression_sync3, pytest.mark.regression_sync4]


def get_test_data(test_data, is_one_frame=True):
    interval, duration, top_left_coordinate, bottom_right_coordinate = test_data
    if is_one_frame:
        return duration, interval, top_left_coordinate, bottom_right_coordinate
    else:
        return interval, duration, top_left_coordinate, bottom_right_coordinate


@pytest.fixture(scope='function')
def prep_for_test_folder(attach_to_app):
    def create_test_obj(dir_name):
        return CaptureFramesTest(dir_name)
    yield create_test_obj


@pytest.fixture(scope='function')
def prep_for_test(attach_to_app):
    obj = CaptureFramesTest(DEF_FOLDER_NAME)
    yield obj
    obj.delete_dir()


@pytest.fixture(scope='function')
def prep_for_neg_test(attach_to_app):
    ssh_client = start()
    yield ssh_client
    ssh_client.close()


@pytest.fixture(scope='function')
def prep_for_neg_test_no_video(prep_for_neg_test):
    execute_command(prep_for_neg_test, cmd.VDP_MEDIA_SERVER_STOP, True)
    yield
    execute_command(prep_for_neg_test, cmd.VDP_MEDIA_SERVER_START, True)


TEST_DATA_SPECIFIC_FRAME = [
    (DEF_INTERVAL, DEF_DURATION, DEF_LEFT_POINT, DEF_RIGHT_POINT),
    (DEF_INTERVAL, DEF_DURATION, ScreenPoint(20, 20), ScreenPoint(10, 10)),
    (DEF_INTERVAL, DEF_DURATION, ScreenPoint(111, 333), ScreenPoint(222, 332)),
    (DEF_INTERVAL, DEF_DURATION, ScreenPoint(1, 2), ScreenPoint(2, 1)),
]

TEST_DATA_FULL_RESOLUTION = [
    (DEF_INTERVAL, DEF_DURATION, DEF_LEFT_POINT, DEF_LEFT_POINT)
]

TEST_DATA_FOLDER_NAMES = [
    'tmp capture  frames',
    '1tmp.capture+frames3',
    '_tmp_Capture_frames',
    '@_WRONG_NAME',
    'tmp_capture_frames/name222',
    r'nam\33',
    'fo|der',
    ' ',
    # ''
]

TEST_DATA_INVALID_RECTANGLE_COORDINATES = [
    (DEF_INTERVAL, DEF_DURATION, ScreenPoint(-10, 10), DEF_RIGHT_POINT),
    (DEF_INTERVAL, DEF_DURATION, DEF_RIGHT_POINT, ScreenPoint(10, -10)),
    (DEF_INTERVAL, DEF_DURATION, ScreenPoint(9999, 9999), ScreenPoint(9999, 9999)),
    (DEF_INTERVAL, DEF_DURATION, ScreenPoint(9999, 10), ScreenPoint(10, 10)),
    (DEF_INTERVAL, DEF_DURATION, ScreenPoint(10, 9999), ScreenPoint(10, 10)),
    (DEF_INTERVAL, DEF_DURATION, ScreenPoint(10, 9999), ScreenPoint(9999, 10)),
    (DEF_INTERVAL, DEF_DURATION, DEF_RIGHT_POINT, DEF_RIGHT_POINT),
    (DEF_INTERVAL, DEF_DURATION, ScreenPoint(10, 20), ScreenPoint(15, 20)),
    (DEF_INTERVAL, DEF_DURATION, ScreenPoint(10, 15), ScreenPoint(10, 20)),
    (DEF_INTERVAL, DEF_DURATION, ScreenPoint(15, 10), ScreenPoint(15, 10)),
]

TEST_DATA_TYPE_ERROR = [
    (None, DEF_DURATION, DEF_RIGHT_POINT, DEF_RIGHT_POINT),
    (DEF_INTERVAL, None, DEF_RIGHT_POINT, DEF_RIGHT_POINT),
    (DEF_INTERVAL, DEF_DURATION, None, DEF_RIGHT_POINT),
    (DEF_INTERVAL, DEF_DURATION, DEF_RIGHT_POINT, None),
    ('', DEF_DURATION, DEF_RIGHT_POINT, DEF_RIGHT_POINT),
    (DEF_INTERVAL, '111', DEF_RIGHT_POINT, DEF_RIGHT_POINT),
    (DEF_INTERVAL, DEF_DURATION, '@#%', DEF_RIGHT_POINT),
    (DEF_INTERVAL, DEF_DURATION, DEF_RIGHT_POINT, ' e33 '),
    (DEF_INTERVAL, DEF_DURATION, True, DEF_RIGHT_POINT),
    (DEF_INTERVAL, DEF_DURATION, DEF_RIGHT_POINT, True),
    (10.3, DEF_DURATION, True, DEF_RIGHT_POINT),
    (DEF_INTERVAL, 10.3, DEF_RIGHT_POINT, True),
    (DEF_INTERVAL, DEF_DURATION, -13.6, DEF_RIGHT_POINT),
    (DEF_INTERVAL, DEF_DURATION, DEF_RIGHT_POINT, -13),
    (DEF_RIGHT_POINT, DEF_DURATION, DEF_RIGHT_POINT, DEF_RIGHT_POINT),
    (DEF_INTERVAL, DEF_RIGHT_POINT, DEF_RIGHT_POINT, DEF_RIGHT_POINT),
]

TEST_DATA_VALUE_ERROR = [
    (-12, DEF_DURATION, DEF_LEFT_POINT, DEF_RIGHT_POINT),
    (DEF_INTERVAL, -13, DEF_LEFT_POINT, DEF_RIGHT_POINT),
]


@jira_test("VHAT-1856")
@pytest.mark.parametrize('test_data', TEST_DATA_SPECIFIC_FRAME)
def test_pos_specific_area_one_frame(prep_for_test, test_data):
    test_data = get_test_data(test_data)
    prep_for_test.do_capture_frames(*test_data) \
        .check_that_files_exist() \
        .check_that_frame_is_one() \
        .check_picture_resolution()


@jira_test("VHAT-1858")
def test_pos_full_resolution_one_frame(prep_for_test):
    prep_for_test.do_capture_frames(DEF_DURATION, DEF_INTERVAL, DEF_LEFT_POINT, DEF_LEFT_POINT)\
        .check_that_files_exist()\
        .check_that_frame_is_one()\
        .check_picture_resolution()


@jira_test("VHAT-1859")
@pytest.mark.parametrize('test_data', TEST_DATA_SPECIFIC_FRAME)
def test_pos_specific_area_several_frames(prep_for_test, test_data):
    test_data = get_test_data(test_data, is_one_frame=False)
    prep_for_test.do_capture_frames(*test_data) \
        .check_that_files_exist() \
        .check_that_frame_is_not_one() \
        .check_picture_resolution()


@jira_test("VHAT-1860")
def test_pos_full_resolution_several_frames(prep_for_test):
    prep_for_test.do_capture_frames(DEF_INTERVAL, DEF_DURATION, DEF_LEFT_POINT, DEF_LEFT_POINT)\
        .check_that_files_exist()\
        .check_that_frame_is_not_one()\
        .check_picture_resolution()


@jira_test("VHAT-1861")
@pytest.mark.parametrize('test_data', TEST_DATA_FOLDER_NAMES)
def test_pos_folder_names(prep_for_test_folder, test_data):
    obj = prep_for_test_folder(test_data)
    try:
        obj.do_capture_frames() \
            .check_that_files_exist() \
            .check_picture_resolution()
    finally:
        logging.info('Tear Down for test. Remove dir [{}]'.format(test_data))
        obj.delete_dir()


@jira_test("VHAT-1862")
def test_run_api_current_performance(prep_for_test):
    prep_for_test.do_capture_frames(MIN_INTERVAL,
                                    MAX_DURATION,
                                    MIN_LEFT_POINT,
                                    ScreenPoint(*prep_for_test.get_current_width_and_height()))\
        .check_that_files_exist()\
        .check_that_frame_is_not_one()\
        .check_picture_resolution()\
        .get_avg_frames()


@jira_test("VHAT-1863")
def test_neg_invalid_duration(prep_for_test):
    with pytest.raises(InvalidDuration) as exc:
        prep_for_test.do_capture_frames(duration=MAX_DURATION+1)
    logging.exception(exc)


@jira_test("VHAT-1864")
def test_neg_permission_denied(prep_for_test):
    execute_command(prep_for_test.ssh_client, cmd.SUDO_MKDIR.format(prep_for_test.path_dir), True, ignore_error=True)
    execute_list_of_commands(prep_for_test.ssh_client, [
        # change rights - all rights(rwx) for owner only
        cmd.SUDO_CHMOD.format(700, prep_for_test.path_dir),
        # change owner - root
        cmd.SUDO_CHOWN.format('root', prep_for_test.path_dir)
    ])
    with pytest.raises(PermissionDenied) as exc:
        try:
            prep_for_test.do_capture_frames()
        finally:
            execute_list_of_commands(prep_for_test.ssh_client, [
                # change owner - nvidia
                cmd.SUDO_CHOWN.format('nvidia', prep_for_test.path_dir),
                # change rights - all rights(rwx) for owner and group of owner
                cmd.SUDO_CHMOD.format(770, prep_for_test.path_dir)])
    logging.exception(exc)


@jira_test("VHAT-1865")
@pytest.mark.parametrize('test_data', TEST_DATA_INVALID_RECTANGLE_COORDINATES)
def test_neg_invalid_rectangle_coordinates(prep_for_test, test_data):
    test_data = get_test_data(test_data, is_one_frame=False)
    with pytest.raises(InvalidRectangleCoordinates) as exc:
        prep_for_test.do_capture_frames(*test_data)
    logging.exception(exc)


@jira_test("VHAT-1866")
@pytest.mark.parametrize('test_data', TEST_DATA_TYPE_ERROR)
def test_neg_type_error(prep_for_neg_test, test_data):
    test_data = get_test_data(test_data)
    with pytest.raises(TypeError) as exc:
        hmi_capture_frames(*test_data, path_directory=DEF_FOLDER_NAME)
    logging.exception(exc)


@jira_test("VHAT-1867")
@pytest.mark.parametrize('test_data', TEST_DATA_VALUE_ERROR)
def test_neg_value_error(prep_for_neg_test, test_data):
    test_data = get_test_data(test_data)
    with pytest.raises(ValueError) as exc:
        hmi_capture_frames(*test_data, path_directory=DEF_FOLDER_NAME)
    logging.exception(exc)


@jira_test("VHAT-1868")
def test_neg_no_video_error(prep_for_neg_test_no_video):
    with pytest.raises(VideoStreamingError) as exc:
        vhat_captureFrames(DEF_INTERVAL, DEF_DURATION, DEF_RIGHT_POINT, DEF_RIGHT_POINT, DEF_FOLDER_NAME)
    logging.exception(exc)


@pytest.mark.skip('Skipped because test would take a lot of time. Run it separate and manually if needed')
@jira_test("VHAT-1869")
def test_neg_no_disk_space(prep_for_test):
    obj = prep_for_test

    def get_avail_space():
        return int(execute_command_and_get_result(obj.ssh_client, cmd.GET_AVAIL_SPACE).split('\r\n')[1])

    with pytest.raises(NoAvailableDiskSpace):
        available_space = get_avail_space()
        try:
            while available_space > 0:
                available_space = get_avail_space()
                logging.debug('available_space: {}'.format(available_space))
                obj.do_capture_frames(interval=MIN_INTERVAL,
                                      duration=MAX_DURATION,
                                      top_left_coordinate=DEF_LEFT_POINT,
                                      bottom_right_coordinate=DEF_LEFT_POINT)
        except NoAvailableDiskSpace as e:
            logging.warning('Doing finally section. Free Space is {}'.format(available_space))
            assert_space = available_space < 350000
            obj.delete_dir()
            logging.warning('Remove directory. Free Space is {}'.format(get_avail_space()))
            logging.exception(e)
            assert assert_space
            raise e
        finally:
            obj.delete_dir()


@jira_test("VHAT-1874")
@pytest.mark.parametrize('test_data', [0, 4, 6])
def test_neg_invalid_arguments(prep_for_neg_test, test_data):
    args = range(test_data)
    with pytest.raises(TypeError) as exc:
        vhat_captureFrames(*args)
    logging.exception(exc)
    assert 'incompatible function arguments' in str(exc.value)
