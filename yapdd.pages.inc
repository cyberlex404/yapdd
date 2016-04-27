<?php

/**
 * @file
 * White label entity editing UI.
 */

/**
 * Yapdd UI controller.
 */
class YapddUIController extends EntityBundleableUIController {

  /**
   * Provides definitions for implementing hook_menu().
   */
  public function hook_menu() {
    $items = parent::hook_menu();

    // Extend the 'add' path.
    $items[$this->path . '/add']['title'] = 'Add ' . yapdd_get_string('entity label');
    return $items;
  }

  /**
   * Operation form submit callback.
   */
  public function operationFormSubmit($form, &$form_state) {
    parent::operationFormSubmit($form, $form_state);
    $form_state['redirect'] = yapdd_get_string('admin menu path content');
  }
}

/**
 * Generates the yapdd editing form.
 */
function yapdd_form($form, &$form_state, Entity $yapdd, $op = 'edit') {

  // Needed by entity_form_field_validate().
  $form['type'] = array('#type' => 'value', '#value' => $yapdd->type);
  //dpm($yapdd->type(), '40 type()');
  $yapdd_domain = $yapdd->type();

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => isset($yapdd->label) ? $yapdd->label : '',
    '#description' => t('The label associated with the @yapdd.', array('@yapdd' => yapdd_get_string('entity label'))),
    '#required' => TRUE,
    '#weight' => -15,
  );

  $form['login'] = array(
    '#type' => 'textfield',
    '#title' => t('Login'),
    '#default_value' => isset($yapdd->login) ? $yapdd->login : '',
    '#description' => t('The login associated with the @yapdd.', array('@yapdd' => yapdd_get_string('entity label'))),
    '#required' => TRUE,
    '#weight' => -16,
    '#field_suffix' => '@' . $yapdd_domain->domain,
  );
/*
  $form['created'] = array(
    '#type' => 'textfield',
    '#title' => t('Creation date'),
    '#default_value' => isset($yapdd->created) ? format_date($yapdd->created, 'short') : '',
    '#description' => t('The date when the @yapdd was created.', array('@yapdd' => yapdd_get_string('entity label'))),
    '#required' => TRUE,
    '#weight' => -14,
  );

  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate this @yapdd', array('@yapdd' => yapdd_get_string('entity label'))),
    '#default_value' => !empty($yapdd->status),
    '#weight' => -10,
  );
*/
  $form['options']['#tree'] = TRUE;
  if ($yapdd->type()->data['supports_revisions'] && empty($yapdd->is_new)) {
    $form['options']['create_revision'] = array('#type' => 'checkbox',
      '#title' => t('Create a new revision'),
      '#default_value' => TRUE,
      '#description' => t('Create a new revision for this edit.'),
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );

  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#weight' => 45,
    '#limit_validation_errors' => array(),
    '#submit' => array('yapdd_form_submit_delete'),
    '#access' => $op != 'add' && $op != 'clone'
  );

  field_attach_form('yapdd', $yapdd, $form, $form_state);

  return $form;
}

/**
 * yapdd editing form validation callback.
 */
function yapdd_form_validate(&$form, &$form_state) {
  entity_form_field_validate('yapdd', $form, $form_state);
}

/**
 * Form API submit callback for the yapdd form.
 */
function yapdd_form_submit(&$form, &$form_state) {
 // $date_format = variable_get('date_format_short', 'm/d/Y - H:i');
 // $date = date_parse_from_format($date_format, $form_state['values']['created']);
 // $form_state['values']['created'] = mktime($date['hour'], $date['minute'], $date['second'], $date['month'], $date['day'], $date['year']);
  $yapdd = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $yapdd->save();
  $form_state['redirect'] = yapdd_get_string('admin menu path content');
}

/**
 * Form API submit callback for the delete button.
 */
function yapdd_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/content/yapdds/' . $form_state['yapdd']->id . '/delete';
}