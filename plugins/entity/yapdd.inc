<?php
/**
 * @file
 * Definition of the yapdd plugin.
 */

$plugin = array(
  'handler' => 'PanelizerEntityWhile',
  'entity path' => yapdd_get_string('base path') . '/%yapdd',
  'uses page manager' => TRUE,
  'hooks' => array(
    'menu' => TRUE,
    'admin_paths' => TRUE,
    'permission' => TRUE,
    'panelizer_defaults' => TRUE,
    'default_page_manager_handlers' => TRUE,
    'form_alter' => TRUE,
    'page_alter' => TRUE,
    'views_data_alter' => TRUE,
    'views_plugins_alter' => TRUE,
  ),
);
