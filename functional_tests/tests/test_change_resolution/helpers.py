import configparser

import requests

OK_STATUS_CODE = 200
URL = 'https://{}/{}'


def change_resolution(resolution):
    display_type = resolution.get('type')
    if not display_type:
        raise TypeError('Type should not be empty')
    elif type(display_type) is not int:
        raise TypeError('Type must be integer, not {}'.format(type(display_type)))
    dataf = {'id': display_type}
    host = _get_config()
    assert requests.post(URL.format(host, 'setDisplayType'), data=dataf, verify=False).status_code == OK_STATUS_CODE
    response = requests.post(URL.format(host, 'getDisplayType'), verify=False)
    assert response.status_code == OK_STATUS_CODE
    return response.json()


def check_resolution_in_response(response, resolution):
    assert resolution.get('height') == response.get('displayHeight')
    assert resolution.get('width') == response.get('displayWidth')


def _get_config():
    configuration = configparser.ConfigParser()
    configuration.read('test_config.ini')
    sync_config = configuration['VDP_BOARD']
    host = sync_config['host']
    return host
