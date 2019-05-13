ATTEMPTS = 100
HOME_ICON = 'home'


def test_recognition_home_icon(driver):
    counter = {'success': 0, 'fail': 0}
    for i in range(ATTEMPTS):
        try:
            driver.wait_for_object(HOME_ICON)
            counter['success'] += 1
            print('Image recognized {} times'.format(counter['success']))
        except LookupError:
            counter['fail'] += 1
            print('Image not recognized {} times'.format(counter['fail']))

    assert (counter['success'] / ATTEMPTS * 100) > 98, "Recognized image < 98%"
    print(counter['success'], counter['fail'])
