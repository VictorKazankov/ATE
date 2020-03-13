import pytest

from functional_tests import config_reader
from functional_tests.utils import ssh_connect

default_folder = config_reader.read_configuration('GET_SCREENSHOT')['location']

folder_name = 'screenshots'

zero_size_file = 'zero.png'

unsupport_type_file = 'unsupport.txt'


@pytest.fixture(scope='module')
def create_temporary_folder(app_connector):
    client = ssh_connect.start()
    ssh_connect.execute_command(client, 'cd {}; sudo rm -r "{}"'.format(default_folder, folder_name),
                                True, True)
    ssh_connect.execute_command(client, 'cd {}; mkdir {}'.format(default_folder, folder_name), True)

    yield client

    ssh_connect.execute_command(client, 'cd {}; sudo rm -r "{}"'.format(default_folder, folder_name),
                                True, True)


@pytest.fixture(scope='function')
def create_zero_size_file(create_temporary_folder):
    client = create_temporary_folder
    # create file zero_size_file.png in screenshots folder, size file = 0
    ssh_connect.execute_command(client, 'cd {}/{}; touch {}'.format(default_folder, folder_name, zero_size_file), True)

    yield "/".join([folder_name, zero_size_file])


@pytest.fixture(scope='function')
def create_unsupport_file(create_temporary_folder):
    client = create_temporary_folder
    # create file .txt type
    path_unsupport_type_file = "/".join([default_folder, folder_name, unsupport_type_file])
    ssh_connect.execute_command(client,
                                'echo "Test file for verify insupport type" > {}'.format(path_unsupport_type_file),
                                True, True)

    yield "/".join([folder_name, unsupport_type_file])
