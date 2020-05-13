import os

import pytest
from functional_tests import config_reader
from functional_tests.utils import ssh_connect
from functional_tests.utils.sftp_connection import SftpConnection

default_folder = config_reader.read_configuration('GET_SCREENSHOT')['location']
folder_name = 'screenshots'
folder_path = '{}/{}'.format(default_folder, folder_name)
images_dir = 'images_to_compare'
zero_size_file = 'zero.png'
unsupport_type_file = 'unsupport.txt'


@pytest.fixture(scope='module')
def create_temporary_folder(app_connector):
    client = ssh_connect.start()
    ssh_connect.execute_command(client, 'rm -r "{}"'.format(folder_path), ignore_error=True)
    ssh_connect.execute_command(client, 'mkdir {}'.format(folder_path))
    yield client
    ssh_connect.execute_command(client, 'rm -r "{}"'.format(folder_path), ignore_error=True)


@pytest.fixture(scope='module')
def copy_test_files_to_board(create_temporary_folder):
    sftp_client = SftpConnection()
    # define path for dir with test images
    dir_path = os.path.join(os.path.dirname(__file__), images_dir)
    # define list of files for dir
    list_of_images = os.listdir(dir_path)
    for image_file in list_of_images:
        # copy file to board
        sftp_client.put_file(source_file=os.path.join(dir_path, image_file),
                             target_file='{}/{}'.format(folder_path, image_file))
    return list_of_images


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
