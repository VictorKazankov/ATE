import configparser

import paramiko as paramiko


def start():
    host, user, passwd = _get_config()
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect(hostname=host, username=user, password=passwd)
    return client


def execute_command(client, command, passwd_required=False):
    stdin, stdout, _ = client.exec_command(command, get_pty=True)
    if passwd_required:
        _, _, passwd = _get_config()
        stdin.write(passwd + '\n')
        stdin.flush()
    exit_status = stdout.channel.recv_exit_status()
    assert exit_status == 0, "Exit status = {}".format(exit_status)
    return stdout


def _get_config():
    configuration = configparser.ConfigParser()
    configuration.read('test_config.ini')
    sync_config = configuration['VDP_BOARD']

    host = sync_config['host']
    user = sync_config['user']
    password = sync_config['passwd']
    return host, user, password
