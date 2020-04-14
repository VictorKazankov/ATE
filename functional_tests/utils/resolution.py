from functional_tests.config_reader import read_configuration_from_file

CONFIG_FILE_NAME = 'sync_display_data.ini'
SECTION_NAME = 'SYNCS'
PARAM_NAME = 'sync_list'


def get_sync_info(cls):
    sync_list = read_configuration_from_file(file_name=CONFIG_FILE_NAME, section=SECTION_NAME, option=PARAM_NAME)
    for sync in sync_list:
        section = read_configuration_from_file(file_name=CONFIG_FILE_NAME, section=sync.upper())
        for attr_name, attr_value in section.items():
            setattr(cls, attr_name.upper(), eval(attr_value))
    return cls


@get_sync_info
class SyncResolution:

    SYNCS = read_configuration_from_file(file_name=CONFIG_FILE_NAME, section=SECTION_NAME, option=PARAM_NAME)

    @staticmethod
    def get_data_for_sync(sync):
        if sync.lower() not in SyncResolution.SYNCS:
            raise ValueError('Wrong sync is provided [{}]. possible values [{}]'.format(sync, SyncResolution.SYNCS))
        list_sync = [attr for attr in dir(SyncResolution) if attr.lower().startswith(sync.lower())]
        return list_sync

    @staticmethod
    def get_list_specific_attr(sync, attr_name):
        list_ = SyncResolution.get_data_for_sync(sync)
        list_attr = [getattr(SyncResolution, attr)[attr_name] for attr in list_ if getattr(SyncResolution, attr)[attr_name]]
        return list_attr

    @staticmethod
    def get_possible_width_for(sync):
        return SyncResolution.get_list_specific_attr(sync, 'Width')

    @staticmethod
    def get_possible_height_for(sync):
        return SyncResolution.get_list_specific_attr(sync, 'Height')

    @staticmethod
    def get_possible_resolution_for(sync):
        return list(zip(
                        SyncResolution.get_possible_width_for(sync),
                        SyncResolution.get_possible_height_for(sync)
        ))

    @staticmethod
    def validate_resolution(sync, width, height):
        if width not in SyncResolution.get_possible_width_for(sync):
            raise ValueError('width for resolution is wrong. Provided width is {}'.format(width))
        if height not in SyncResolution.get_possible_height_for(sync):
            raise ValueError('height for resolution is wrong. Provided height is {}'.format(height))

    @staticmethod
    def get_type_for_resolution(sync, width, height, diagonal):
        SyncResolution.validate_resolution(sync, width, height)

        list_type = list(zip(
                        SyncResolution.get_possible_width_for(sync),                # 0 - width
                        SyncResolution.get_possible_height_for(sync),               # 1 - height
                        SyncResolution.get_list_specific_attr(sync, 'Type'),        # 2 - type
                        SyncResolution.get_list_specific_attr(sync, 'Diagonal')     # 3 - diagonal
        ))

        pos_variants = [item for item in list_type
                        if item[0] == width and item[1] == height and str(diagonal) in item[3]]

        if len(pos_variants) == 1:
            return pos_variants[0][2]
        else:
            raise ValueError('Several values are match for the mentioned conditions [{}]'.format(pos_variants))
