ATTEMPTS = 100
HOME_ICON = 'home'


def test_recognition_home_icon(driver):
    count_recognition_home_icon = {'success': 0, 'fail': 0}
    for i in range(ATTEMPTS):
        try:
            driver.wait_for_object_api(HOME_ICON)
            count_recognition_home_icon['success'] += 1
            print('Image recognized {} times'.format(count_recognition_home_icon['success']))
        except LookupError:
            count_recognition_home_icon['fail'] += 1
            print('Image not recognized {} times'.format(count_recognition_home_icon['fail']))

    assert (count_recognition_home_icon['success'] / ATTEMPTS * 100) > 98, "Recognized image < 98%"
    print(count_recognition_home_icon['success'], count_recognition_home_icon['fail'])
