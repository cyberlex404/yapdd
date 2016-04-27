<?php

/**
 * @file
 * Handle the 'yapdd view' override task.
 *
 * This plugin overrides yapdd/%entity_object and reroutes it to the page manager, where
 * a list of tasks can be used to service this request based upon criteria
 * supplied by access plugins.
 */

/**
 * Specialized implementation of hook_page_manager_task_tasks(). See api-task.html for
 * more information.
 */
function yapdd_yapdd_view_page_manager_tasks() {
  $path = yapdd_yapdd_view_path();

  return array(
    // This is a 'page' task and will fall under the page admin UI
    'task type' => 'page',

    'title' => t('@yapdd template', array('@yapdd' => yapdd_get_string('entity label'))),

    'admin title' => t('@yapdd template', array('@yapdd' => yapdd_get_string('entity label'))),
    'admin description' => t('When enabled, this overrides the default Drupal behavior for displaying @yapdds at <em>@path</em>. If you add variants, you may use selection criteria such as @yapdd type or language or user access to provide different views of @yapdds. If no variant is selected, the default Drupal @yapdd view will be used. This page only affects @yapdds viewed as pages, it will not affect @yapdds viewed in lists or at other locations. Also please note that if you are using pathauto, aliases may make a yapdd to be somewhere else, but as far as Drupal is concerned, they are still at @path.', array('@path' => $path, '@yapdd' => yapdd_get_string('entity label'), '@yapdds' => yapdd_get_string('entity plural label'))),
    'admin path' => $path,

    // Menu hooks so that we can alter the yapdd/%entity_object menu entry to point to us.
    'hook menu alter' => 'yapdd_yapdd_view_menu_alter',

    // This is task uses 'context' handlers and must implement these to give the
    // handler data it needs.
    'handler type' => 'context',
    'get arguments' => 'yapdd_yapdd_view_get_arguments',
    'get context placeholders' => 'yapdd_yapdd_view_get_contexts',

    // Allow this to be enabled or disabled:
    'disabled' => variable_get('yapdd_yapdd_view_disabled', TRUE),
    'enable callback' => 'yapdd_yapdd_view_enable',
    'access callback' => 'yapdd_yapdd_view_access_check',
  );
}

/**
 * Callback defined by yapdd_yapdd_view_page_manager_tasks().
 *
 * Alter the yapdd view input so that yapdd view comes to us rather than the
 * normal yapdd view process.
 */
function yapdd_yapdd_view_menu_alter(&$items, $task) {
  if (variable_get('yapdd_yapdd_view_disabled', TRUE)) {
    return;
  }

  $path = yapdd_yapdd_view_path();

  // Override the yapdd view handler for our purpose.
  $callback = $items[$path]['page callback'];
  if ($callback == 'entity_ui_entity_page_view' || variable_get('yapdd_override_anyway', FALSE)) {
    $items[$path]['page callback'] = 'yapdd_yapdd_view_page';
    $items[$path]['file path'] = $task['path'];
    $items[$path]['file'] = $task['file'];
  }
  else {
    // automatically disable this task if it cannot be enabled.
    variable_set('yapdd_yapdd_view_disabled', TRUE);
    if (!empty($GLOBALS['yapdd_enabling_yapdd_view'])) {
      drupal_set_message(t('Page manager module is unable to enable %path because some other module already has overridden with %callback.', array('%path' => $path, '%callback' => $callback)), 'error');
    }
  }

  // @todo override yapdd revision handler as well?
}

/**
 * Entry point for our overridden yapdd view.
 *
 * This function asks its assigned handlers who, if anyone, would like
 * to run with it. If no one does, it passes through to Drupal core's
 * yapdd view, which is yapdd_page_view().
 */
function yapdd_yapdd_view_page($yapdd) {
  // Load my task plugin
  $task = page_manager_get_task('yapdd_view');

  // Load the yapdd into a context.
  ctools_include('context');
  ctools_include('context-task-handler');

  // We need to mimic Drupal's behavior of setting the yapdd title here.
  drupal_set_title($yapdd->label());
  $uri = entity_uri('yapdd', $yapdd);
  // Set the yapdd path as the canonical URL to prevent duplicate content.
  drupal_add_html_head_link(array('rel' => 'canonical', 'href' => url($uri['path'], $uri['options'])), TRUE);
  // Set the non-aliased path as a default shortlink.
  drupal_add_html_head_link(array('rel' => 'shortlink', 'href' => url($uri['path'], array_merge($uri['options'], array('alias' => TRUE)))), TRUE);
  $contexts = ctools_context_handler_get_task_contexts($task, '', array($yapdd));

  $output = ctools_context_handler_render($task, '', $contexts, array($yapdd->id));
  if ($output != FALSE) {
    // todo
    //    node_tag_new($node);
    return $output;
  }

  $function = 'entity_ui_entity_page_view';
  foreach (module_implements('page_manager_override') as $module) {
    $call = $module . '_page_manager_override';
    if (($rc = $call('entity_view')) && function_exists($rc)) {
      $function = $rc;
      break;
    }
  }

  // Otherwise, fall back.
  return $function($yapdd);
}

/**
 * Callback to get arguments provided by this task handler.
 *
 * Since this is the yapdd view and there is no UI on the arguments, we
 * create dummy arguments that contain the needed data.
 */
function yapdd_yapdd_view_get_arguments($task, $subtask_id) {
  return array(
    array(
      'keyword' => 'yapdd',
      'identifier' => t('@yapdd being viewed', array('@yapdd' => yapdd_get_string('entity label'))),
      'id' => 1,
      'name' => 'entity_id:yapdd',
      'settings' => array(),
    ),
  );
}

/**
 * Callback to get context placeholders provided by this handler.
 */
function yapdd_yapdd_view_get_contexts($task, $subtask_id) {
  return ctools_context_get_placeholders_from_argument(yapdd_yapdd_view_get_arguments($task, $subtask_id));
}

/**
 * Callback to enable/disable the page from the UI.
 */
function yapdd_yapdd_view_enable($cache, $status) {
  variable_set('yapdd_yapdd_view_disabled', $status);

  // Set a global flag so that the menu routine knows it needs
  // to set a message if enabling cannot be done.
  if (!$status) {
    $GLOBALS['yapdd_enabling_yapdd_view'] = TRUE;
  }
}

/**
 * Callback to determine if a page is accessible.
 *
 * @param $task
 *   The task plugin.
 * @param $subtask_id
 *   The subtask id
 * @param $contexts
 *   The contexts loaded for the task.
 * @return
 *   TRUE if the current user can access the page.
 */
function yapdd_yapdd_view_access_check($task, $subtask_id, $contexts) {
  $context = reset($contexts);
  return yapdd_access('view', $context->data);
}

/**
 * Helper function that provides the override path.
 */
function yapdd_yapdd_view_path() {
  $info = entity_get_info('yapdd');
  $wildcard = isset($info['admin ui']['menu wildcard']) ? $info['admin ui']['menu wildcard'] : '%entity_object';
  return yapdd_get_string('base path') . '/' . $wildcard;
}