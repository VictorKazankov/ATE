# This file is used to measure recognition time for get_exist
import logging

from functional_tests.utils.sync3.constants import Icons as sync3_icon
from functional_tests.utils.sync3.constants import Text as sync3_text
from functional_tests.utils.sync4.constants import Icons as sync4_icon
from functional_tests.utils.sync4.constants import Text as sync4_text

text_time = []
image_time = []


def set_time(time, object_name):
    if isinstance(object_name, str):
        if object_name in sync4_icon.__dict__.values() or sync3_icon.__dict__.values():
            image_time.append(time)
            logging.info('Recognition time of {} image: {}'.format(object_name, time))
        elif object_name in sync4_text.__dict__.values() or sync3_text.__dict__.values():
            text_time.append(time)
            logging.info('Recognition time of {} text: {}'.format(object_name, time))
        else:
            logging.info('Object is not defined')
    else:
        logging.info('Object is not string')


def get_text_average_time():
    return calculate_average_time(text_time)


def get_image_average_time():
    return calculate_average_time(image_time)


def calculate_average_time(data_set):
    if data_set:
        return reduce(lambda a, b: a + b, data_set) / float(len(data_set))
