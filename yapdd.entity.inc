<?php

/**
 * @file
 * Holds entity related classes.
 */

/**
 * The yapdd entity controller.
 */
class YapddController extends EntityAPIController {

  /**
   * Overridden to add in more default values.
   */
  public function create(array $values = array()) {
    $values += array(
      'created' => REQUEST_TIME,
      'langcode' => LANGUAGE_NONE,
    );
    return parent::create($values);
  }
}


/**
 * A yapdd entity.
 */
class YapddEntity extends Entity {

  public $created;
  public $changed;
  public $id;
  public $type;
  public $label = '';
  public $login = '';
  public $status = 1;

  public function __construct($values = array()) {
    // Support creation with the type object also.
    if (isset($values['type']) && is_object($values['type'])) {
      $values['type'] = $values['type']->name;
    }
    parent::__construct($values, 'yapdd');
  }

  /**
   * Overridden to care about created and changed times.
   */
  public function save() {
    // Do not automatically set a created values for already existing entities.
    if (empty($this->created) && (!empty($this->is_new) || !$this->id)) {
      $this->created = REQUEST_TIME;
    }
    //$this->login = 'new new new';

    $yapdd_domain = $this->type();
    if ($yapdd_domain->supportsRevisions()) {
      $this->is_new_revision = isset($this->options) && !empty($this->options['create_revision']);
    }
    $this->changed = REQUEST_TIME;

    parent::save();
  }

  /**
   * Gets the associated yapdd type object.
   *
   * @return YapddType
   */
  public function type() {
    return yapdd_domain_load($this->type);
  }

  /**
   * Implements a custom default URI
   */
  public function defaultUri() {
    return array(
      'path' => yapdd_get_string('base path') . '/' . $this->id,
    );
  }
}

/**
 * A yapdd type entity.
 * Domain YapddDomain == 
 */
class YapddDomain extends Entity {

  public $label = '';
  public $domain = '';
  public $token = '';
  public $weight = 0;
  public $api_url = 'https://pddimp.yandex.ru/api2/admin/';

  /**
   * Helper method to check if the yapdd type supports revisions.
   *
   * @return bool
   *   TRUE if revision support is enabled, FALSE if not.
   */
  public function supportsRevisions() {
    return !empty($this->data['supports_revisions']);
  }

  public function domainDetails() {
    $op = 'domain/details?';
    $url = $this->api_url . $op;

    $data_arr = array(
      'domain' => $this->domain,
    );
    $host = $url . drupal_http_build_query($data_arr);
    dpm($host);

    $result = drupal_http_request($host, array(
      'headers' => array(
        'PddToken' => $this->token,
        'Content-Type' => 'application/x-www-form-urlencoded',
      ),
      'method' => 'GET',
      //'data' => drupal_http_build_query($data_arr),
    ));
    $data = drupal_json_decode($result->data);
    dpm($data, 'status');

    if(isset($data['error']) || $data['success'] == 'error') {
      // housadd_set_error($data['error']);
    }
    return $data;
  }

}