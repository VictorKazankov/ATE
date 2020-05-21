# This file is used to measure recognition time for get_exist
import logging

from functional_tests.utils.menlo.constants import Icons as menlo_icon
from functional_tests.utils.menlo.constants import Text as menlo_text
from functional_tests.utils.sync3.constants import Icons as sync3_icon
from functional_tests.utils.sync3.constants import Text as sync3_text
from functional_tests.utils.sync4.constants import Icons as sync4_icon
from functional_tests.utils.sync4.constants import Text as sync4_text

text_time = []
image_time = []


def set_time(time, object_name):
    if isinstance(object_name, str):
        if any(object_name in icons.__dict__.values() for icons in (sync3_icon, sync4_icon, menlo_icon)):
            image_time.append(time)
            logging.info('Recognition time of {} image: {}'.format(object_name, time))
        elif any(object_name in icons.__dict__.values() for icons in (sync3_text, sync4_text, menlo_text)):
            text_time.append(time)
            logging.info('Recognition time of {} text: {}'.format(object_name, time))
        else:
            logging.error('object is not defined by {}'.format(object_name))
    else:
        logging.error('object_name type is not string, but {}'.format(type(object_name)))


def get_text_average_time():
    return calculate_average_time(text_time)


def get_image_average_time():
    return calculate_average_time(image_time)


def calculate_average_time(data_set):
    if data_set:
        return reduce(lambda a, b: a + b, data_set) / float(len(data_set))
