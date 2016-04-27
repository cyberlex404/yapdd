<?php
/**
 * @file
 * while.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function yapdd_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'yapdds';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'yapdd_base';
  $view->human_name = 'White label entity types';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = yapdd_get_string('entity plural label');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer yapdds';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
    'type' => 'type',
    'label' => 'label',
    'created' => 'created',
    'status' => 'status',
    'id_1' => 'id_1',
    'id_2' => 'id_1',
  );
  $handler->display->display_options['style_options']['default'] = 'id';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'label' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'id_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => ' ',
      'empty_column' => 0,
    ),
    'id_2' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: White label entity: White label entity ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'yapdd_base';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = 'ID';
  /* Field: White label entity: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'yapdd_base';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: White label entity: URL */
  $handler->display->display_options['fields']['url']['id'] = 'url';
  $handler->display->display_options['fields']['url']['table'] = 'views_entity_yapdd';
  $handler->display->display_options['fields']['url']['field'] = 'url';
  $handler->display->display_options['fields']['url']['exclude'] = TRUE;
  $handler->display->display_options['fields']['url']['link_to_entity'] = 0;
  /* Field: White label entity: Label */
  $handler->display->display_options['fields']['label_1']['id'] = 'label_1';
  $handler->display->display_options['fields']['label_1']['table'] = 'yapdd_base';
  $handler->display->display_options['fields']['label_1']['field'] = 'label';
  $handler->display->display_options['fields']['label_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['label_1']['alter']['path'] = '[url]';
  /* Field: White label entity: Date created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'yapdd_base';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: White label entity: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'yapdd_base';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Is valid';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: White label entity: White label entity ID */
  $handler->display->display_options['fields']['id_1']['id'] = 'id_1';
  $handler->display->display_options['fields']['id_1']['table'] = 'yapdd_base';
  $handler->display->display_options['fields']['id_1']['field'] = 'id';
  $handler->display->display_options['fields']['id_1']['label'] = 'Operations';
  $handler->display->display_options['fields']['id_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['id_1']['alter']['text'] = 'edit';
  $handler->display->display_options['fields']['id_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['id_1']['alter']['path'] = yapdd_get_string('base path') . '/[id]/edit';
  /* Field: yapdd: yapdd ID */
  $handler->display->display_options['fields']['id_2']['id'] = 'id_2';
  $handler->display->display_options['fields']['id_2']['table'] = 'yapdd_base';
  $handler->display->display_options['fields']['id_2']['field'] = 'id';
  $handler->display->display_options['fields']['id_2']['label'] = 'Delete';
  $handler->display->display_options['fields']['id_2']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['id_2']['alter']['text'] = 'delete';
  $handler->display->display_options['fields']['id_2']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['id_2']['alter']['path'] = yapdd_get_string('base path') . '/[id]/delete';
  $handler->display->display_options['fields']['id_2']['element_label_colon'] = FALSE;
  /* Filter criterion: White label entity: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'yapdd_base';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = yapdd_get_string('admin menu path content');
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = yapdd_get_string('entity plural label');
  $handler->display->display_options['menu']['weight'] = '2';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['yapdds'] = $view;

  return $export;
}
