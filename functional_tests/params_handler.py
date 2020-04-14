from functional_tests.config_reader import \
    read_configuration_from_file as __READ_CONFIG_FROM_FILE
from functional_tests.utils.ATE_exception_handing import \
    finish_pytest_runs as __finish_pytest_runs

_support_params = ["sync", "build", "col_mode", "width", "height", "dialonal", "config_file"]
support_keys = {'--{}'.format(item): item for item in _support_params}
__mapping_for_server_config = {'sync': 'Target', 'build': 'Build', 'col_mode': 'CollectionMode',
                               'width': 'FrameWidth', 'height': 'FrameHeight', 'dialonal': 'Diagonal'}

__TEXT_FOR_EXIT_PYTEST = 'Cancel pytest execution because no data provided for {}'
__HOST = None
__SYNC = None


def __prep_config_file(console_params):
    return_dict = dict()
    for key, value in __mapping_for_server_config.items():
        return_dict[value] = console_params[key]
    return return_dict


def _define_configuration(config, params=None):
    config_to_return = __READ_CONFIG_FROM_FILE(file_name='def_sync_config.ini', section='DEF_SYNC_CONFIG',
                                               option='{}_DEF'.format(__SYNC.upper()))
    tmp_text = ''
    item_text = 'using specific {} [{}] '

    if params:
        for item in params:
            config_to_return[__mapping_for_server_config[item]] = config.get(item)
            text = item_text.format(item, config.get(item))
            tmp_text = '{}{}'.format(tmp_text, text)

    text_to_return = 'Def configuration for sync [{}] {} will be used [{}]'\
        .format(config.get('sync'), tmp_text, config_to_return)

    return config_to_return, text_to_return


def __apply_prioritise(config):
    global __SYNC
    __CONFIG = None
    __TEXT = None

    if config.get('config_file'):
        __CONFIG = __READ_CONFIG_FROM_FILE(config.get('config_file'), section='DEFAULT', option='CONFIG')
        __SYNC = __CONFIG.get(__mapping_for_server_config.get('sync'))
        __TEXT = 'Config file will be used [{}]'.format(__CONFIG)

    else:
        __SYNC = config.get('sync')

        if all([value for item, value in config.items() if item in __mapping_for_server_config.keys()]):
            __CONFIG = __prep_config_file(config)
            __TEXT = 'Manual configuration will be used [{}]'.format(__CONFIG)

        elif config.get('sync'):
            __CONFIG, __TEXT = _define_configuration(config, [key for key, value in config.items() if value])

        else:
            __finish_pytest_runs(__TEXT_FOR_EXIT_PYTEST.format('sync'))
            raise EnvironmentError
    return __CONFIG, __TEXT


def get_config_for_params(console_params):
    return __apply_prioritise(console_params)


def get_host():
    if __HOST:
        return __HOST
    else:
        __finish_pytest_runs(__TEXT_FOR_EXIT_PYTEST.format('host'))
        raise EnvironmentError


def get_sync():
    if __SYNC:
        return __SYNC
    else:
        __finish_pytest_runs(__TEXT_FOR_EXIT_PYTEST.format('sync'))
        raise EnvironmentError
