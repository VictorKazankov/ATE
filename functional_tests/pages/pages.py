import time


class HomePage:
    def __init__(self, hmi):
        self.hmi = hmi

    def open_home_page(self):
        try:
            self.hmi.wait_for_object('home')
            self.hmi.mouse_press('home')
        except LookupError:
            try:
                self.hmi.wait_for_object('back')
                self.hmi.mouse_press('back')
                self.hmi.mouse_press('home')
            except LookupError:
                try:
                    self.hmi.wait_for_object('back_property_field')
                    self.hmi.mouse_press('back_property_field')
                    self.hmi.mouse_press('back')
                    self.hmi.mouse_press('home')
                except LookupError:
                    try:
                        self.hmi.wait_for_object('close')
                        self.hmi.mouse_press('close')
                        self.hmi.mouse_press('back')
                        self.hmi.mouse_press('home')
                    except LookupError:
                        print("Can't goto page")

    def verify_action_home_page(self):
        try:
            self.hmi.wait_for_object('Navigation fault')
        except LookupError:
            return False


class ClimatePage:
    def __init__(self, hmi):
        self.hmi = hmi

    def open_climate_page(self):
        if self.hmi.wait_for_object('climate'):
            self.hmi.mouse_press('climate')
        else:
            print("Can't go to Climate page")

    def verify_active_climate_page(self):
        try:
            self.hmi.wait_for_object('climate_active')
        except LookupError:
            return False

    def verify_active_ac_control_button(self):
        try:
            self.hmi.wait_for_object('climate_on_off')
        except LookupError:
            self.hmi.mouse_press('AUTO')


class AudioPage:
    def __init__(self, hmi):
        self.hmi = hmi

    def open_audio_page(self):
        if self.hmi.wait_for_object('audio'):
            self.hmi.mouse_press('audio')
        else:
            print("Can't go to Audio page")

    def verify_active_audio_page(self):
        try:
            self.hmi.wait_for_object('audio_active')
        except LookupError:
            return False

    def verify_active_105_50_frequency(self):
        try:
            time.sleep(1)
            self.hmi.wait_for_object('105.50')
            print("found")
        except LookupError:
            self.hmi.mouse_press('audio_frequency')
            self.hmi.mouse_press('key_1_active')
            self.hmi.mouse_press('key_0_active')
            self.hmi.mouse_press('key_5_active')
            self.hmi.mouse_press('key_5_active')
            self.hmi.mouse_press('key_0_active')
            self.hmi.mouse_press('enter_active')


class PhonePage:
    def __init__(self, hmi):
        self.hmi = hmi

    def open_phone_page(self):
        if self.hmi.wait_for_object('phone'):
            self.hmi.mouse_press('phone')
        else:
            print("Can't go to Phone page")

    def verify_active_phone_page(self):
        try:
            self.hmi.wait_for_object('phone_active')
        except LookupError:
            return False


class SettingsPage:
    def __init__(self, hmi):
        self.hmi = hmi

    def open_settings_page(self):
        try:
            self.hmi.wait_for_object('settings')
            self.hmi.mouse_press('settings')
        except LookupError:
            try:
                self.hmi.wait_for_object('settings_active')
                self.hmi.mouse_press('settings_active')
            except LookupError:
                print("Can't go to settings page")

    def verify_active_setting_page(self):
        try:
            self.hmi.wait_for_object('settings_sound')
        except LookupError:
            return False

    def open_setting_clock_page(self):
        if self.hmi.wait_for_object('clock'):
            self.hmi.mouse_press('clock')
        else:
            print("Can't go to Settings clock page")

    def open_setting_audio_page(self):
        if self.hmi.wait_for_object('settings_sound'):
            self.hmi.mouse_press('settings_sound')
        else:
            print("Can't go to Settings clock page")


class SettingsClockPage:
    def __init__(self, hmi):
        self.hmi = hmi

    def verify_active_setting_clock_page(self):
        try:
            self.hmi.wait_for_object('Clock settings')
        except LookupError:
            return False

    def verify_active_12h_button(self):
        try:
            self.hmi.wait_for_object('12h')
        except LookupError:
            self.hmi.mouse_press('24h')

    def open_information_dialog(self):
        if self.hmi.wait_for_object('information'):
            self.hmi.mouse_press('information')
        else:
            print("Can't go to Info dialog")

    def close_information_dialog(self):
        try:
            self.hmi.wait_for_object('close')
            self.hmi.mouse_press('close')
        except LookupError:
            print("Can't close information dialog")


class SettingsAudioPage:
    def __init__(self, hmi):
        self.hmi = hmi

    def verify_active_setting_audio_page(self):
        try:
            self.hmi.wait_for_object('Sound settings')
        except LookupError:
            return False

    def open_settings_audio_property_field_page(self):
        try:
            self.hmi.wait_for_object('medium')
            self.hmi.mouse_press('medium')
        except LookupError:
            print("Can't go to property field_page")

    '''
    Methods verify activities pages
    '''
