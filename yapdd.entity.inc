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
      dpm($values);
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

  public function domain() {
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
// email/get_oauth_token
  public function getOauthToken() {
    $domain = $this->domain();
    $domain->domainDetails();
    dpm($this);
    $info = array(
      'domain' => $domain->domain,
      'login' => $this->login,
    );
    $url = 'email/get_oauth_token';
    $data = $domain->yapddApiRequest($url, $info);
    if(!empty($data)) {
      return $data['oauth-token'];
    }else{
      return FALSE;
    }
  }

  public function Oauth() {
    $token = $this->getOauthToken();
    $host = 'https://passport.yandex.ru/passport?';
    if($token) {
      $info = array(
        'mode' => 'oauth',
        'access_token' => $token,
        'type' => 'trusted-pdd-partner',
      );
      $host = $host . drupal_http_build_query($info);
      $request_conf = array(
        'headers' => array(
          'Content-Type' => 'application/x-www-form-urlencoded',
        ),
        'method' => 'GET',
      );
      drupal_goto($host);

      //$result = drupal_http_request($host, $request_conf);
      //$data = drupal_json_decode($result->data);
      //dpm($data);
    }
  }


  public function yapddApiRequest($url, $info = array(), $method = 'POST') {
    if(empty($info)) {
      $info['domain'] = $this->domain;
    }
    $host = YAPDD_API . $url;
    $request_conf = array(
      'headers' => array(
        'PddToken' => $this->token,
        'Content-Type' => 'application/x-www-form-urlencoded',
      ),
      'method' => $method,
    );

    if($method == 'GET') {
      $host = $host . drupal_http_build_query($info);
    }else {
      $request_conf['data'] = drupal_http_build_query($info);
    }
    // dpm($request_conf, $host);
    $result = drupal_http_request($host, $request_conf);
    $data = drupal_json_decode($result->data);
    dpm($data, 'yapddApiRequest: ' . $method . ':' . $url . drupal_http_build_query($info));

    if(isset($data['error']) || $data['success'] == 'error') {
      // housadd_set_error($data['error']);
    }
    return $data;

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
  public $next_check = 0;
  public $last_check = 0;
  public $check_results = '';
  public $country = '';
  public $default_theme = '';
  public $imap_enabled = 0;
  public $pop_enabled = 0;
  public $delegated = '';
  public $logo_url = '';
  //public $api_url = 'https://pddimp.yandex.ru/api2/admin/';

  /**
   * Helper method to check if the yapdd type supports revisions.
   *
   * @return bool
   *   TRUE if revision support is enabled, FALSE if not.
   */
  public function supportsRevisions() {
    return !empty($this->data['supports_revisions']);
  }

  public function yapddApiRequest($url, $info = array(), $method = 'POST') {
    if(empty($info)) {
      $info['domain'] = $this->domain;
    }
    $host = YAPDD_API . $url;
    $request_conf = array(
      'headers' => array(
        'PddToken' => $this->token,
        'Content-Type' => 'application/x-www-form-urlencoded',
      ),
      'method' => $method,
    );

    if($method == 'GET') {
      $host = $host . drupal_http_build_query($info);
    }else {
      $request_conf['data'] = drupal_http_build_query($info);
    }
    // dpm($request_conf, $host);
    $result = drupal_http_request($host, $request_conf);
    $data = drupal_json_decode($result->data);
    dpm($data, 'yapddApiRequest: ' . $method . ':' . $url . drupal_http_build_query($info));

    if(isset($data['error']) || $data['success'] == 'error') {
      // housadd_set_error($data['error']);
    }
    return $data;

  }


  /*
   * See https://tech.yandex.ru/pdd/doc/reference/domain-details-docpage/
   */
  public function domainDetails() {
    $method = 'domain/details?';

    $data = $this->yapddApiRequest($method, array(), 'GET');
    if($data['success'] == 'ok') {
      $this->imap_enabled = $data['imap_enabled'];
      $this->pop_enabled = $data['pop_enabled'];
      $this->country = $data['country'];
      $this->delegated = $data['delegated'];
      $this->logo_url = !empty($data['logo_url']) ? $data['logo_url'] : '';
      $this->save();
    }
    dpm($this);

    return $data;
  }


  public function domainStatus() {
    $method = 'domain/registration_status?';
    $data = $this->yapddApiRequest($method, array(),'GET');
    if($data['success'] == 'ok') {
      $this->last_check = strtotime($data['last_check']);
      $this->next_check = strtotime($data['next_check']);
      $this->check_results = $data['check_results'];
      ///dpm($data,$this->last_check);
      $this->save();
      // TODO Добавить domain_status
    }
    return $data;
  }

  public function domainDNSGet() {
    $method = 'dns/list?';
    $data = array(
      'domain' => $this->domain,
    );
    $data = $this->yapddApiRequest($method);
    return $data;
  }

  // dns/add
  public function domainDNSAdd($record) {
    $method = 'dns/add';
    $info = array(
      'domain' => $this->domain,
    );
    $data = $this->yapddApiRequest($method, $info);
  }


  /**
   * Работа с email
   */

  public function emailAdd($login, $pass = NULL) {
    // POST
    if(is_null($pass)) {
      $pass = user_password();
    }
    $info = array(
      'domain' => $this->domain,
      'login' => $login,
      'password' => $pass,
    );
    $method = 'email/add';
    return $this->yapddApiRequest($method, $info);
  }

  public function emailList() {
    $first = $this->emailListPage();

    if(isset($first)) {
      $total = $first['total'];

      $accounts = $first['accounts'];
      $load = count($accounts);

      $i = 2;
      while ($load < $total) {
        $package = $this->emailListPage($i);
        //dpm($package);
        $i++;
        if($package) {
          $load = $load + count($package['accounts']);
          $accounts = array_merge($accounts, $package['accounts']);
          //dpm($accounts, 'after +');
        }
      }
      return $accounts;
    }
    return $first;
  }

  private function emailListPage($page = 1) {
    $on_page = 30;
    $method = 'email/list?';

    $info = array(
      'domain' => $this->domain,
      'page' => $page,
      'on_page' => $on_page,
    );
    return $this->yapddApiRequest($method, $info, 'GET');
  }

}