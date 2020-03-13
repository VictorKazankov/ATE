from vhat_client import CollectionMode

sync_options = ['sync_wild', '*nc_wi*', 'sync_wi*',
                '*nc_wild', '*', '?ync_wild',
                'syn?_wild', 'sync_wil?', '?ync_wi*',
                '*nc_wil?']

build_options = ['test_wildcard_build', '*st_wildcard_bu*', 'test_wildcard_bu*',
                 '*st_wildcard_build', '*', '?est_wildcard_build',
                 'tes?_wildcard_build', 'test_wildcard_buil?', '?est_wildcard_bu*',
                 '*st_wildcard_buil?']

mode_options = [CollectionMode.DAY, CollectionMode.ANY]

name_options = ['test_wildcard_name', '*st_wildcard_na*', 'test_wildcard_na*',
                '*st_wildcard_name', '*', '?est_wildcard_name',
                'tes?_wildcard_name', 'test_wildcard_nam?', '?est_wildcard_na*',
                '*st_wildcard_nam?']

parent_options = ['test_wildcard_parent', '*st_wildcard_pa*', 'test_wildcard_pa*',
                  '*st_wildcard_parent', '*', '?est_wildcard_parent',
                  'tes?_wildcard_parent', 'test_wildcard_paren?', '?est_wildcard_pa*',
                  '*st_wildcard_paren?']
