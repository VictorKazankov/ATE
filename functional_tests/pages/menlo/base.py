import vhat_client

from functional_tests.pages.hmi import obj_exists, wait_for_object


class SwipeError(RuntimeError):
    pass


class PageNotLoadedError(RuntimeError):
    pass


class BasePage(object):

    @property
    def _load_indicators(self):
        raise NotImplementedError('{} load indicators are not assigned'.format(self.__class__.__name__))

    @property
    def is_active(self):
        return any(obj_exists(indicator) for indicator in self._load_indicators)

    def wait_for_page_to_load(self):
        for indicator in self._load_indicators:
            try:
                wait_for_object(indicator)
                return
            except vhat_client.LookupError:
                pass
        raise PageNotLoadedError('{} page unable to open'.format(self.__class__.__name__))
