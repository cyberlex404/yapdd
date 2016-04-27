<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class YapddMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['type'] = array(
      'type' => 'yapdd_domain',
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer yapdds',
      'required' => TRUE,
      'description' => yapdd_get_string('type label'),
    ) + $properties['type'];

    $properties['label'] = array(
      'type' => 'text',
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
      'label' => t('Label'),
      'schema field' => 'label',
    );

    $properties['user'] = array(
      'label' => t("User"),
      'type' => 'user',
      'description' => t("The owner of the @yapdd.", array('@yapdd' => yapdd_get_string('entity label'))),
      'getter callback' => 'entity_property_getter_method',
      'setter callback' => 'entity_property_setter_method',
      'setter permission' => 'administer yapdds',
      'required' => TRUE,
      'schema field' => 'uid',
    );

    $properties['created'] = array(
      'label' => t("Date created"),
      'type' => 'date',
      'description' => t("The date the @yapdd was created.", array('@yapdd' => yapdd_get_string('entity label'))),
      'setter callback' => 'entity_property_verbatim_set',
      'setter permission' => 'administer yapdds',
      'schema field' => 'created',
    );

    $properties['changed'] = array(
      'label' => t("Date changed"),
      'type' => 'date',
      'schema field' => 'changed',
      'description' => t("The date the @yapdd was most recently updated.", array('@yapdd' => yapdd_get_string('entity label'))),
    );

    $properties['status'] = array(
      'label' => t("Status"),
      'type' => 'boolean',
      'schema field' => 'status',
      'description' => t("Whether the @yapdd is active (true) or not (false).", array('@yapdd' => yapdd_get_string('entity label'))),
    );

    return $info;
  }
}
