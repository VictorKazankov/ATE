import paramiko as paramiko

from functional_tests.config_reader import read_configuration


def start():
    host, user, passwd = _get_config()
    client = paramiko.SSHClient()
    client.set_missing_host_key_policy(paramiko.AutoAddPolicy())
    client.connect(hostname=host, username=user, password=passwd)
    return client


def execute_command(client, command, passwd_required=False, ignore_error=False):
    stdin, stdout, _ = client.exec_command(command, get_pty=True)
    if passwd_required:
        _, _, passwd = _get_config()
        stdin.write(passwd + '\n')
        stdin.flush()
    exit_status = stdout.channel.recv_exit_status()
    if not ignore_error:
        assert exit_status == 0, "Exit status = {}".format(exit_status)
    return stdout


def _get_config():
    sync_config = read_configuration('VDP_BOARD')
    host = sync_config['host']
    user = sync_config['user']
    password = sync_config['passwd']
    return host, user, password
