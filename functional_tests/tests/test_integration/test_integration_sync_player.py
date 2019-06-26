import logging
import time

import pytest


@pytest.mark.integration_sync3
def test_integration_sync3(driver_sync3):
    logging.info("Open home page")
    if not driver_sync3.home_page.home_page_is_active():
        driver_sync3.home_page.open_home_page()
    logging.info("Open audio page")
    driver_sync3.audio_page.open_audio_page()
    logging.info("Sleep for 10 seconds")
    time.sleep(10)


@pytest.mark.integration_sync4
def test_integration_sync4(driver_sync4):
    logging.info("Open settings page")
    if not driver_sync4.settings_page.settings_page_is_active():
        driver_sync4.settings_page.open_settings_page()
    logging.info("Open settings clock page")
    driver_sync4.settings_page.open_setting_clock_page()
    logging.info("Sleep for 10 seconds")
    time.sleep(10)
