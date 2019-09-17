import time

import allure
import pytest
from functional_tests.pages.hmi import attach_to_application
from functional_tests.utils import ssh_connect
from functional_tests.utils.sync3.constants import TASK_LINK

KILLALL = 'sudo killall -9 vhat_server'
RESTART = 'sudo systemctl restart vhat_server'
START = 'sudo systemctl start vhat_server'
STOP = 'sudo systemctl stop vhat_server'
STOP_MEDIASERVER = 'sudo systemctl stop run_media_srv.service'
START_MEDIASERVER = 'sudo systemctl start run_media_srv.service'
START_TCP = 'nc -l localhost 5000'
RETRIEVE_LOG = "journalctl | grep 'Failed to start VHAT'"
FIND_PORT = 'lsof -i :5000'
KILL_PID = 'sudo kill {}'
CURRENT_DATE = 'date'

SLEEP = 5


@allure.testcase(TASK_LINK.format("VHAT-559"), "VHAT-559")
@pytest.mark.failover
def test_check_exception_after_stop_vhat():
    client = ssh_connect.start()
    ssh_connect.execute_command(client, STOP, passwd_required=True)
    with pytest.raises(RuntimeError):
        attach_to_application()
    ssh_connect.execute_command(client, RESTART, passwd_required=True)


@allure.testcase(TASK_LINK.format("VHAT-559"), "VHAT-559")
@pytest.mark.failover
def test_recovery():
    client = ssh_connect.start()
    ssh_connect.execute_command(client, KILLALL, passwd_required=True)
    attach_to_application()


@allure.testcase(TASK_LINK.format("VHAT-559"), "VHAT-559")
@pytest.mark.failover
def test_port_busy_another_process():
    client = ssh_connect.start()
    ssh_connect.execute_command(client, STOP, passwd_required=True)
    output = ssh_connect.execute_command(client, CURRENT_DATE)
    curr_time = output.read()[4:16]

    client.exec_command(START_TCP, get_pty=False)
    ssh_connect.execute_command(client, START, passwd_required=True)
    time.sleep(SLEEP)
    logs = ssh_connect.execute_command(client, RETRIEVE_LOG)
    process_time = logs.read().split('\n')[-2][0:12]
    find_process_on_port_and_kill(client)
    time.sleep(SLEEP)
    ssh_connect.execute_command(client, START, passwd_required=True)
    attach_to_application()
    assert curr_time == process_time


@pytest.mark.skip(reason="vhat is stuck when there is no mediaserver")
@allure.testcase(TASK_LINK.format("VHAT-559"), "VHAT-559")
@pytest.mark.timeout(60)
def test_stop_media_server(driver_sync3):
    client = ssh_connect.start()
    ssh_connect.execute_command(client, STOP_MEDIASERVER, passwd_required=True)
    attach_to_application()
    if not driver_sync3.home_page.home_page_is_active():
        driver_sync3.home_page.open_home_page()
    ssh_connect.execute_command(client, START_MEDIASERVER, passwd_required=True)
    if not driver_sync3.home_page.home_page_is_active():
        driver_sync3.home_page.open_home_page()


def find_process_on_port_and_kill(client):
    ports = ssh_connect.execute_command(client, FIND_PORT)
    data = ports.read().split('\n')
    processes = [pid[8:13] for pid in data[1:-1]]
    for process in processes:
        ssh_connect.execute_command(client, KILL_PID.format(process), passwd_required=True)
