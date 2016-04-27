<?php
/**
 * Created by PhpStorm.
 * User: lex
 * Date: 27.04.16
 * Time: 14:48
 */

function yapdd_domain_config($form, &$form_state) {
  $form['#tree'] = TRUE;
  $conf = variable_get('yapdd');
  yapdd_domain_status();
  $form['yapdd'] = array(
    '#type' => 'fieldset',
    '#title' => 'Yandex PDD',
  );

  $form['yapdd']['domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Domain'),
    '#default_value' => $conf['domain'],
    '#required' => TRUE,
  );

  $form['yapdd']['token'] = array(
    '#type' => 'textfield',
    '#title' => t('PDD token'),
    '#default_value' => $conf['token'],
  );
  return system_settings_form($form);
}