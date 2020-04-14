from time import sleep
import requests
from functional_tests.params_handler import get_host


HTTP_SUCCESS_CODE = 200
URL = 'https://{}/{}'
WAIT_TIME = 10


class TSHMI_API(object):

    def __new__(cls, *args, **kwargs):
        if not hasattr(cls, 'instance'):
            cls.instance = super(TSHMI_API, cls).__new__(cls)
        return cls.instance

    def __init__(self):
        self.HOST = get_host()

    def _do_post(self, api_name, payload, verify=False):
        try:
            response = requests.post(url=URL.format(self.HOST, api_name), json=payload, verify=verify)
            if response.status_code == 500 and response.text.startswith('Another user with IP'):
                raise EnvironmentError(response.text)
            else:
                return response
        except requests.exceptions.ConnectionError as e:
            raise e

    def change_resolution(self, display_type, exp_res_code=HTTP_SUCCESS_CODE):
        payload = {'id': display_type}
        response = self._do_post('setDisplayType', payload)
        if response.status_code == exp_res_code:
            sleep(WAIT_TIME)  # wait for rebooting sync
            return True
        else:
            return False

    def get_resolution(self, exp_res_code=HTTP_SUCCESS_CODE):
        payload = {}
        response = self._do_post('getDisplayType', payload)
        if response.status_code == exp_res_code:
            return response.json()
        else:
            return False
