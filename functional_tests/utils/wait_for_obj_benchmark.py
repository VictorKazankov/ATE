# This file is used to measure recognition time of waitForObject
from functional_tests.utils.sync3 import constants as sync3_const
from functional_tests.utils.sync4 import constants as sync4_const

text_time = []
image_time = []


def set_time(time, object_name):
    if isinstance(object_name, str):
        if object_name in sync3_const.Icons.__dict__.values():
            image_time.append(time)
        elif object_name in sync4_const.Icons.__dict__.values():
            image_time.append(time)
        else:
            text_time.append(time)


def get_text_average_time():
    return calculate_average_time(text_time)


def get_image_average_time():
    return calculate_average_time(image_time)


def calculate_average_time(data_set):
    if data_set:
        return reduce(lambda a, b: a + b, data_set) / float(len(data_set))
