from time import sleep

from functional_tests.tests.helpers import get_exist_result
from functional_tests.utils import ssh_connect

SIGHUP = 'sudo kill -SIGHUP {}'
GET_PID = "ps -aux | grep \"$(which ate_server)\" | grep root | awk '{print $2}'"


def test_object_recognition_after_change_image_name_in_json_file(settings_change_icon_storage):
    # call SIGHUP for reloaf icon_storage
    find_and_kill_sighup_vhat_procces()
    sleep(2)
    # verify that image recognized after reload for test image
    assert get_exist_result('test_phone_add_phone')


def find_and_kill_sighup_vhat_procces():
    client = ssh_connect.start()

    stdout = client.exec_command(GET_PID)[1]
    pid_vhat_server = stdout.read()[:-1]
    call_sighup_signal = SIGHUP.format(pid_vhat_server)
    ssh_connect.execute_command(client, call_sighup_signal, passwd_required=True)
