from time import sleep

import allure
import pytest
from functional_tests import config_reader
from functional_tests.utils.ssh_connect import execute_command, start
from functional_tests.utils.sync3.constants import TASK_LINK
from vhat_client import LookupError, VideoStreamingError
from vhat_client import attachToApplication as attach_to_application
from vhat_client import waitForObject as wait_for_object

STOP_STREAMER = 'sudo systemctl stop vdp_media_server.service'
START_STREAMER = 'sudo systemctl start vdp_media_server.service'


@pytest.fixture(scope='module', autouse=True)
def client_setup():
    client = start()
    return client


class GpioService():
    def __init__(self, client_setup):
        self._client = client_setup
        self._gpio_pin = config_reader.read_configuration('VDP_BOARD')[
            'gpio_pin']
        self._gpio_path = '/sys/class/gpio'
        self._video_path = '{}/gpio{}'.format(self._gpio_path, self._gpio_pin)

    def is_enable(self):

        check_dir_cmd = r'[ -d {} ] && echo yes || echo no'.format(
            self._video_path)
        is_dir_exist = execute_command(self._client, check_dir_cmd,
                                       ignore_error=True).read().strip()
        return is_dir_exist == 'yes'

    def is_started(self):

        edge_path = r'{}/edge'.format(self._video_path)
        get_edge_cmd = r'[ -f {} ] && cat {}'.format(
            edge_path, edge_path)
        gpio_edge = execute_command(self._client, get_edge_cmd,
                                    ignore_error=True).read().strip()

        return gpio_edge == 'both'

    def enable(self):
        if self.is_enable():
            return
        enable_gpio_cmd = r'echo {} | sudo tee {}/export'.format(
            self._gpio_pin, self._gpio_path)
        execute_command(self._client, enable_gpio_cmd, passwd_required=True)
        # gpio availability is read only once as server init, restart needed
        self.restart_ate_server()

    def disable(self):
        if not self.is_enable():
            return
        disable_gpio_cmd = r'echo {} | sudo tee {}/unexport > /dev/null'.format(
            self._gpio_pin, self._gpio_path)
        execute_command(self._client, disable_gpio_cmd, passwd_required=True)
        # gpio availability is read only once as server init, restart needed
        self.restart_ate_server()

    def start(self):
        if self.is_started():
            return
        direction_path = r'{}/direction'.format(self._video_path)
        set_direction_cmd = r'echo in | sudo tee {}  > /dev/null'.format(
            direction_path)
        edge_path = r'{}/edge'.format(self._video_path)
        set_edge_cmd = r'echo both | sudo tee {}  > /dev/null'.format(
            edge_path)

        execute_command(self._client, set_direction_cmd, passwd_required=True)
        execute_command(self._client, set_edge_cmd, passwd_required=True)

    def stop(self):
        if not self.is_started():
            return
        edge_path = r'{}/edge'.format(self._video_path)
        set_edge_cmd = r'echo none | sudo tee {}  > /dev/null'.format(
            edge_path)

        direction_path = r'{}/direction'.format(self._video_path)
        set_direction_cmd = r'echo out | sudo tee {}  > /dev/null'.format(
            direction_path)

        execute_command(self._client, set_edge_cmd, passwd_required=True)
        execute_command(self._client, set_direction_cmd, passwd_required=True)

    def restart_ate_server(self):
        restart_cmd = 'sudo systemctl restart vdp_ate_server'
        execute_command(self._client, restart_cmd, passwd_required=True)
        sleep(1)


@pytest.fixture(scope='module')
def gpio_config(client_setup):
    config = GpioService(client_setup)
    yield config
    config.enable()
    config.start()


@allure.testcase(TASK_LINK.format("VHAT-1329"), "VHAT-1329")
def test_video_stream_streamer_stopped_raise_exception(client_setup):
    execute_command(client_setup, STOP_STREAMER, passwd_required=True)
    with pytest.raises(VideoStreamingError):
        wait_for_object('main_settings_button_active')
    execute_command(client_setup, START_STREAMER, passwd_required=True)
    sleep(.5)


@allure.testcase(TASK_LINK.format("VHAT-1329"), "VHAT-1329")
def test_video_stream_disable_gpio_no_exception(gpio_config):
    try:
        gpio_config.disable()
        attach_to_application('')
        wait_for_object('main_settings_button_active')
    except VideoStreamingError:
        pytest.fail('Unexpected VideoStreamingError')
    except LookupError:
        pass
    finally:
        gpio_config.enable()


@allure.testcase(TASK_LINK.format("VHAT-1329"), "VHAT-1329")
def test_video_stream_stop_gpio_raise_exception(gpio_config):
    gpio_config.stop()
    with pytest.raises(VideoStreamingError):
        attach_to_application('')
        wait_for_object('main_settings_button_active')
    gpio_config.start()
