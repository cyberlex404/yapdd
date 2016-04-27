<?php
/**
 * Created by PhpStorm.
 * User: lex
 * Date: 27.04.16
 * Time: 15:28
 */
define("YAPDD_API" , 'https://pddimp.yandex.ru/api2/admin/');

function yapdd_domain_status() {
  $conf = variable_get('yapdd');
  // https://pddimp.yandex.ru/api2/admin/domain/registration_status
  $url = 'https://pddimp.yandex.ru/api2/admin/domain/registration_status?';

  $data_arr = array(
    'domain' => $conf['domain'],
  );
  $host = $url . drupal_http_build_query($data_arr);
  dpm($host);

  $result = drupal_http_request($host, array(
    'headers' => array(
      'PddToken' => $conf['token'],
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


function yapdd_email_add($login, $pass = NULL) {
  // https://pddimp.yandex.ru/api2/admin/email/add
  $op = 'email/add';
  $method = 'POST';

  $conf = variable_get('yapdd', NULL);
  if(is_null($pass)) {
    $pass = user_password();
  }
  $data_arr = array(
    'domain' => $conf['domain'],
    'login' => $login,
    'password' => $pass,
  );

  $result = drupal_http_request(YAPDD_API . $op, array(
    'headers' => array(
      'PddToken' => $conf['token'],
      'Content-Type' => 'application/x-www-form-urlencoded',
    ),
    'method' => $method,
    'data' => drupal_http_build_query($data_arr),
  ));
  $data = drupal_json_decode($result->data);
  dpm($data, 'add mail');

  if(isset($data['success']) &&  $data['success'] == 'ok') {
    drupal_set_message('Почтовый ящик ' . $data['login'] . 'создан.  Пароль: ' .$pass );
  }
  if(isset($data['error']) || $data['success'] == 'error') {
    //housadd_set_error($data['error']);
  }
  return $data;
}