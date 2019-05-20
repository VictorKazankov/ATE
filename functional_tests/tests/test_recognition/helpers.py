class Helpers:
    def __init__(self, hmi):
        self.pages = hmi

    '''
    Recognition method
    '''

    def get_result_recognition(self, name, dict_attemps):
        attempts = 10
        for i in range(attempts):
            try:
                # if recognition is successfull we increase our counter of success on one
                self.pages.wait_for_object(name)
                dict_attemps['success'] += 1
                print('Image recognized {} times'.format(dict_attemps['success']))
            except LookupError:
                # if recognition is not successfull we reduce our counter of success on one
                dict_attemps['fail'] += 1
                print('Image not recognized {} times'.format(dict_attemps['fail']))

        return dict_attemps, attempts
