from functional_tests.pages import hmi


def get_result_recognition(n, name, dict_attemps):
    for i in range(n):
        try:
            # if recognition is successfull we increase our counter of success on one
            hmi.wait_for_object(name)
            dict_attemps['success'] += 1
            print('Image recognized {} times'.format(dict_attemps['success']))
        except LookupError:
            # if recognition is not successfull we reduce our counter of success on one
            dict_attemps['fail'] += 1
            print('Image not recognized {} times'.format(dict_attemps['fail']))

    return dict_attemps
