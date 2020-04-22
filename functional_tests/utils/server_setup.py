from vhat_client import (CollectionMode, InvalidSyncBuildVersion,
                         InvalidSyncCollectionMode, InvalidSyncVersion)
from functional_tests.config_reader import read_configuration_from_file
from functional_tests.pages.hmi import (change_sync_icon_db, change_sync_mode)
from functional_tests.utils.TSHMI_API import TSHMI_API
from functional_tests.utils.resolution import SyncResolution
from functional_tests.utils.ssh_connect import execute_command, start, execute_list_of_commands
from functional_tests.utils import ssh_commands as cmd


_server_config_bkp = 'ate_server.ini.bkp'
_config_path = cmd.ATE_SERVER_CONFIG
_cmd_get_data = '''cat {} | egrep "FrameWidth|FrameHeight|Target =|Build =|CollectionMode ="'''

_tmp_config_bkp_path = '~/{}'.format(_server_config_bkp)
_tmp_config_path = '~/ate_server.ini'
_octal_permissions_notation = 666

_attr_name_sync_v = 'Target'
_attr_name_build = 'Build'
_attr_name_collection_mode = 'CollectionMode'
_attr_name_width = 'FrameWidth'
_attr_name_height = 'FrameHeight'
_attr_name_diagonal = 'Diagonal'
_exc_message = 'Config data does NOT contain attribute [{}] or value is incorrect, provided [{}], possible [{}]'

_CONFIG_FILE_NAME = 'sync_display_data.ini'
_SECTION_NAME = 'SYNCS'
_MODE_LIST = read_configuration_from_file(file_name=_CONFIG_FILE_NAME, section=_SECTION_NAME, option='mode_list')
_DIAGONAL_LIST = read_configuration_from_file(file_name=_CONFIG_FILE_NAME, section=_SECTION_NAME, option='diagonal_list')


class EnvSetup(object):
    """
    Example of using:
    obj = EnvSetup({'Target': 'sync3', 'Build': 'version3.2_build17288_devtest', 'CollectionMode': 'day_mode',
                 'FrameWidth': 800, 'FrameHeight': 480, 'Diagonal': 6}).apply_new_config()

    obj = EnvSetup(C.SYNC3_DEF).setup_repo(Repo.SYSTEM)
    """

    def __init__(self, config):
        if type(config) is not dict:
            raise TypeError('Wrong type, should be dict with parameters')
        self.client = start()

        self.config = config
        self.sync_v = config.get(_attr_name_sync_v, None)
        self.build = config.get(_attr_name_build, None)
        self.collection_mode = config.get(_attr_name_collection_mode, None)
        self.width = config.get(_attr_name_width)
        self.height = config.get(_attr_name_height)
        self.diagonal = config.get(_attr_name_diagonal, None)

        self.display_type_id = SyncResolution.get_type_for_resolution(self.sync_v, self.width, self.height, self.diagonal)
        self._init_config = self.get_ate_server_ini()
        self._init_resolution = None
        self._config_for_cmd = config.copy()
        self._config_for_cmd.pop(_attr_name_diagonal)

    def __del__(self):
        self.client.close()

    @property
    def sync_v(self):
        return self._sync_v

    @sync_v.setter
    def sync_v(self, value):
        if value is None or value not in SyncResolution.SYNCS:
            raise ValueError(_exc_message.format(_attr_name_sync_v, value, SyncResolution.SYNCS))
        else:
            self._sync_v = value

    @property
    def build(self):
        return self._build

    @build.setter
    def build(self, value):
        if value is None:
            raise ValueError(_exc_message.format(_attr_name_build, value, 'A lot of variants :)'))
        else:
            try:
                change_sync_icon_db(self.sync_v, value)
            except InvalidSyncVersion as e:
                self.__roll_back(e)
            except InvalidSyncBuildVersion as e:
                self.__roll_back(e)
            self._build = value

    @property
    def collection_mode(self):
        return self._collection_mode

    @collection_mode.setter
    def collection_mode(self, value):
        if value is None or value not in _MODE_LIST:
            raise ValueError(_exc_message.format(_attr_name_collection_mode, value, _MODE_LIST))
        else:
            try:
                if value == 'day_mode':
                    change_sync_mode(CollectionMode.DAY)
                elif value == 'night_mode':
                    change_sync_mode(CollectionMode.NIGHT)
            except InvalidSyncCollectionMode as e:
                self.__roll_back(e)
            self._collection_mode = value

    @property
    def width(self):
        return self._width

    @width.setter
    def width(self, value):
        if value not in SyncResolution.get_possible_width_for(self.sync_v):
            raise ValueError(_exc_message.format(_attr_name_width, value,
                                                     set(SyncResolution.get_possible_width_for(self.sync_v))))
        else:
            self._width = value

    @property
    def height(self):
        return self._height

    @height.setter
    def height(self, value):
        if value not in SyncResolution.get_possible_height_for(self.sync_v):
            raise ValueError(_exc_message.format(_attr_name_width, value,
                                                     set(SyncResolution.get_possible_height_for(self.sync_v))))
        else:
            self._height = value

    @property
    def diagonal(self):
        return self._diagonal

    @diagonal.setter
    def diagonal(self, value):
        if value is None or value not in _DIAGONAL_LIST:
            raise ValueError(_exc_message.format(_attr_name_diagonal, value, _DIAGONAL_LIST))
        else:
            self._diagonal = value

    def get_ate_server_ini(self, file_path=_config_path, raw_data=False):

        tmp_info = execute_command(self.client, _cmd_get_data.format(file_path)).read()

        raw_list_params = tmp_info.strip().split('\r\n')
        if raw_data:
            return raw_list_params
        else:
            return {item.split('=')[0].strip(): item.split('=')[1].strip() for item in raw_list_params}

    def __prep_command(self):
        raw_params = self.get_ate_server_ini(raw_data=True)
        changes = ''
        for param, new_value in self._config_for_cmd.items():
            text_to_change = [item for item in raw_params if item.startswith(param)][0]
            change = 's/{}/{}/; '.format(text_to_change, '{} = {}'.format(param, new_value))
            changes = '{}{}'.format(changes, change)
        return "sed -e '{}' {} > {}".format(changes, _config_path, _tmp_config_path)

    def __apply_new_settings(self, command):

        execute_list_of_commands(self.client, [
            cmd.SUDO_TOUCH.format(_tmp_config_path),
            cmd.SUDO_CHMOD.format(_octal_permissions_notation, _tmp_config_path),
            command,
            cmd.SUDO_COPY_FILE.format(_tmp_config_path, _config_path),
            # TODO Wait bug fix VHAT-1825
            # cmd.VDP_ATE_SERVER_RESTART
        ])

    @staticmethod
    def get_current_resolution():
        return TSHMI_API().get_resolution()

    def __change_resolution_on_sync_by_type(self):
        TSHMI_API().change_resolution(self.display_type_id)

    def __change_resolution_sync3(self):
        client = start()
        command = cmd.SYNC3_SH.format(self.display_type_id)
        with client.invoke_shell() as ssh:
            ssh.send(command)
        client.close()

    def __change_resolution_sync4(self):
        self._init_resolution = self.get_current_resolution()
        self.__change_resolution_on_sync_by_type()

    def __apply_sync_specific_logic(self):
        if self.sync_v == 'sync3':
            self.__change_resolution_sync3()
        elif self.sync_v == 'sync4':
            self.__change_resolution_sync4()
        else:
            raise EnvironmentError('Wrong sync version, provided [{}]'.format(self.sync_v))

    def apply_new_config(self):
        command = self.__prep_command()
        self.__prep_roll_back()
        try:
            self.__apply_new_settings(command)
            self.__apply_sync_specific_logic()
        except Exception as e:
            self.__roll_back(e)

    def __prep_roll_back(self):
        execute_list_of_commands(self.client, [
            cmd.SUDO_TOUCH.format(_tmp_config_bkp_path),
            cmd.SUDO_CHMOD.format(_octal_permissions_notation, _tmp_config_bkp_path),
            cmd.SUDO_COPY_FILE.format(_config_path, _tmp_config_bkp_path)
        ])

    def __roll_back(self, exc):
        execute_list_of_commands(self.client, [
            cmd.SUDO_COPY_FILE.format(_tmp_config_bkp_path, _config_path),
            cmd.VDP_ATE_SERVER_RESTART
        ])
        raise exc
