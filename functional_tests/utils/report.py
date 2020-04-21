import allure

from functional_tests.tests.import_helper_functions import data

_JIRA_TICKET_LINK = 'https://adc.luxoft.com/jira/browse/{}'


def _str_for_data(args):
    text = ', '.join(args)
    return text.rstrip(', ')


def _get_complete_description(initial, jira_id, test_data):
    """
    to get formatted description
    :param initial: initial description
    :param jira_id: id of the ticket in jira
    :param test_data: iterable to parametrize description
    :return: description string
    """
    complete = '{} {}'.format(jira_id, initial) if initial else jira_id
    if test_data:
        return '{} for {}'.format(complete, _str_for_data(test_data))
    return complete


def jira_issue(jira_id=None, description=None, test_data=None, path=None, name=None):
    """
    decorator for issue specifying in allure report

    If the issue is not environment specific
    jira_id, description and test_data should be specified directly
    during the decoration.
    Example:
    @jira_issue('VHAT-123')
    @jira_issue('VHAT-123', 'sample description',['text1','text2'])

    Otherwise if the issue is environment specific,
    decorator should be called with path and name params specified
    for dynamically import the issue details
    Please note: there is no necessity to declare issue attribute
    in test_data module for all environments used (stubbed or etc)
     - create it only where it makes sense.
     Examples:
      test_data module attribute:
        vhat_123=('VHAT-123', 'sample description',['text1','text2'])
      usage in tests:
        @jira_issue(path='test_exists', name='vhat_123')

    :param jira_id: id of the defect ticket in JIRA
    :param description: text specified in allure report for the issue
    :param test_data: iterable of data reproduce the issue
    :param path: environment specific package name
     to look for test_data module
    :param name: name of the attribute in test_data module
     to look for details
    :return: allure.issue function call result
    """

    def wrapper(function):
        return function

    # getting data for environment specific issue
    if path and name:
        try:
            jira_id, description, test_data = data(path, name)
        # no issue mentioned in report if unable to find data for current environment
        except AttributeError:
            return wrapper
    # no issue mentioned in report if no jira_id specified
    if not jira_id:
        return wrapper
    description = _get_complete_description(description, jira_id, test_data)
    return allure.issue(_JIRA_TICKET_LINK.format(jira_id), description)


def jira_test(jira_id):
    """
    decorator wrapping allure decorator to display link to the test task
    :param jira_id: JIRA ID string
    :return: allure method to display in report
    """
    return allure.testcase(_JIRA_TICKET_LINK.format(jira_id), jira_id)
