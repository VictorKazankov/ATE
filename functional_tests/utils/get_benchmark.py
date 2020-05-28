# This file is used to measure recognition time for get_exist
import csv
import logging

import allure

from functional_tests.pages.menlo.constants import Icons as menlo_icon
from functional_tests.pages.menlo.constants import Text as menlo_text
from functional_tests.utils.path_joiner import path_joiner
from functional_tests.utils.sync3.constants import Icons as sync3_icon
from functional_tests.utils.sync3.constants import Text as sync3_text
from functional_tests.utils.sync4.constants import Icons as sync4_icon
from functional_tests.utils.sync4.constants import Text as sync4_text

text_time = list()
image_time = list()
stat_list = list()


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


def save_stat_to_file(file_name='recognition_time.csv'):
    file_path = path_joiner('allure-results', file_name)
    logging.info('stats recognition is writen to {}'.format(file_path))

    with open(file_path, 'wb') as csv_file:
        writer = csv.writer(csv_file, delimiter=';', quotechar='"', quoting=csv.QUOTE_MINIMAL)
        writer.writerow(['image', 'Avg', get_image_average_time()])
        writer.writerow(['text', 'Avg', get_text_average_time()])
        writer.writerows(stat_list)
    return file_path


def attach_stats_file_to_allure(file_name):
    def read_file(_file_name):
        with open(_file_name, 'rb') as csv_file:
            return csv_file.read()

    allure.attach(read_file(file_name), name='Stats Recognition', attachment_type=allure.attachment_type.CSV)
