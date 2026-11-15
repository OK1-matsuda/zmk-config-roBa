/*
 * オートマウスレイヤー (AML) 中にマウスクリック・修飾キー以外のキーが
 * 押されたら、AML を抜ける。
 *
 * ここでは keycode_state_changed を購読し、通常キーの押下時に同レイヤーを無効化する。
 *  - マウスクリック (&mkp) は keycode イベントを発生させないため対象外
 *  - 修飾キー (LCTRL 等、mod-tap のホールド含む) は is_mod() で除外
 *  - レイヤーキー (&mo, &lt のホールド) も keycode イベントを発生させないため対象外
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/keymap.h>
#include <zmk/keys.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#define TRACKBALL_NODE DT_NODELABEL(trackball)

#if DT_NODE_HAS_PROP(TRACKBALL_NODE, automouse_layer)
#define AML_LAYER DT_PROP(TRACKBALL_NODE, automouse_layer)
#else
#define AML_LAYER 0
#endif

#if AML_LAYER > 0

static int aml_exit_on_keypress_listener(const zmk_event_t *eh) {
    const struct zmk_keycode_state_changed *ev = as_zmk_keycode_state_changed(eh);
    if (ev == NULL || !ev->state) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    // 修飾キー単体の押下では抜けない
    if (is_mod(ev->usage_page, ev->keycode)) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    if (zmk_keymap_layer_active(AML_LAYER)) {
        LOG_DBG("AML exit by keycode 0x%02X (page 0x%02X)", ev->keycode, ev->usage_page);
        zmk_keymap_layer_deactivate(AML_LAYER);
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(aml_exit_on_keypress, aml_exit_on_keypress_listener);
ZMK_SUBSCRIPTION(aml_exit_on_keypress, zmk_keycode_state_changed);

#endif
