# レイヤー状態とキーコードイベントは central 側にしか存在しないため、central (または非分割) のみでビルドする
if(CONFIG_ZMK_POINTING AND ((NOT CONFIG_ZMK_SPLIT) OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL))
  target_sources(app PRIVATE src/aml_exit_on_keypress.c)
endif()
