<?php
/**
 * Created by PhpStorm.
 * User: lex
 * Date: 27.04.16
 * Time: 14:48
 */
include_once 'yapdd.api.inc';

function yapdd_domain_config($form, &$form_state) {
  $form['#tree'] = TRUE;
  $conf = variable_get('yapdd');
  yapdd_domain_status();
  yapdd_email_add(time());
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


/**
 * UI controller.
 */
class WhileTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = yapdd_get_string('admin menu description');
    return $items;
  }
}

/**
 * Generates the yapdd type editing form.
 */
function yapdd_type_form($form, &$form_state, Entity $yapdd_type, $op = 'edit') {

  if ($op == 'clone') {
    $yapdd_type->label .= ' (cloned)';
    $yapdd_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $yapdd_type->label,
    '#description' => t('The human-readable name of this @yapdd-type.', array('@yapdd-type' => yapdd_get_string('type label'))),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($yapdd_type->name) ? $yapdd_type->name : '',
    '#maxlength' => 32,
    '#disabled' => $yapdd_type->hasStatus(ENTITY_IN_CODE) && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'yapdd_type_load',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this @yapdd-type. It must only contain lowercase letters, numbers, and underscores.', array('@yapdd-type' => yapdd_get_string('type label'))),
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $yapdd_type->weight,
    '#description' => t('When showing @yapdds, those with lighter (smaller) weights get listed before @yapdds with heavier (larger) weights.', array('@yapdds' => yapdd_get_string('entity plural label'))),
    '#weight' => 10,
  );

  $form['data']['#tree'] = TRUE;
  $form['data']['supports_revisions'] = array('#type' => 'checkbox',
    '#title' => t('Support revisions'),
    '#default_value' => !empty($yapdd_type->data['supports_revisions']),
    '#description' => t('Enable revision support for this @yapdd-type.', array('@yapdd-type' => yapdd_get_string('type label'))),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save @yapdd-type', array('@yapdd-type' => yapdd_get_string('type label'))),
    '#weight' => 40,
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete @yapdd-type', array('@yapdd-type' => yapdd_get_string('type label'))),
    '#weight' => 45,
    '#limit_validation_errors' => array(),
    '#submit' => array('yapdd_type_form_submit_delete'),
    '#access' => !$yapdd_type->hasStatus(ENTITY_IN_CODE) && $op != 'add' && $op != 'clone'
  );
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function yapdd_type_form_submit(&$form, &$form_state) {
  $yapdd_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $yapdd_type->save();
  $form_state['redirect'] = yapdd_get_string('admin menu path');
}

/**
 * Form API submit callback for the delete button.
 */
function yapdd_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = yapdd_get_string('admin menu path') . '/' . $form_state['yapdd_type']->name . '/delete';
}