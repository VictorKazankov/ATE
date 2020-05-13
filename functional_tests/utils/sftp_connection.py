from functional_tests.utils.ssh_connect import start
from scp import SCPClient


class SftpConnection:

    def __init__(self, ssh_connection=None):
        self._ssh = ssh_connection if ssh_connection else start()
        self._sftp = SCPClient(self._ssh.get_transport())

    def __del__(self):
        self._sftp.close()
        self._ssh.close()

    def get_file(self, source_file, target_file):
        self._sftp.get(source_file, target_file)

    def put_file(self, source_file, target_file):
        self._sftp.put(source_file, target_file)
