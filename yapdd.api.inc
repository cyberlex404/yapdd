<?php
/**
 * Created by PhpStorm.
 * User: lex
 * Date: 27.04.16
 * Time: 15:28
 */

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
      'PddToken' => $conf['pdd_token'],
      'Content-Type' => 'application/x-www-form-urlencoded',
    ),
    'method' => 'GET',
    //'data' => drupal_http_build_query($data_arr),
  ));
  $data = drupal_json_decode($result->data);
//  dpm($data, 'add mail');

  if(isset($data['error']) || $data['success'] == 'error') {
   // housadd_set_error($data['error']);
  }
  return $data;
}