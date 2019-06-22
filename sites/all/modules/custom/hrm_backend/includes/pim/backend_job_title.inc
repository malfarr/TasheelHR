<?php
/*
 * Organization Structure
 */

function hr_backend_job_title_organization_structure() {
  global $user;
  $account = hr_basic_account_get_info($user->uid);

  return theme('page_wrapper_organization_structure', array('account' => $account));
}

function hr_backend_job_title_structure_form($form, &$form_state, $account) {

  $form['jobs']['#tree'] = TRUE;

  $jobs = db_select('hrm_config_job_title', 'hrm_config_job_title')
          ->fields('hrm_config_job_title', array('id', 'name'))
          ->condition('org_id', $account['organization'], '=')
          ->execute()->fetchAllKeyed(0, 1);

  $root_items = db_select('hr_organization_structure', 'organization_structure')
          ->fields('organization_structure', array('job_id', 'parent', 'weight'))
          ->condition('org_id', $account['organization'], '=')
          ->condition('parent', 0, '=')
          ->orderBy('weight', 'ASC')
          ->execute()->fetchAllAssoc('job_id', PDO::FETCH_ASSOC);


  $items = array();
  $depth = -1;

  foreach ($root_items as $root_item) {
    hr_backend_job_title_structure_get_tree($jobs, $root_item, $items, $depth);
  }

  foreach ($items as $job_id => $item) {
    $form['jobs'][$job_id] = array(
      'name' => array(
        '#markup' => $item['name'],
      ),
      'id' => array(
        '#type' => 'hidden',
        '#default_value' => $item['id'],
      ),
      'pid' => array(
        '#type' => 'hidden',
        '#default_value' => $item['parent'],
      ),
      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $item['weight'],
        '#delta' => 100,
        '#title_display' => 'invisible',
      ),
      'depth' => array(
        '#type' => 'hidden',
        '#value' => $item['depth'],
      ),
    );
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save Changes'));
  return $form;
}

function hr_backend_job_title_structure_form_submit($form, &$form_state) {

  $account = $form_state['build_info']['args'][0];

  foreach ($form_state['values']['jobs'] as $id => $item) {
    db_update('hr_organization_structure')
        ->fields(array('parent' => $item['pid'], 'weight' => $item['weight'], 'uid' => $account['uid'], 'created' => time()))
        ->condition('org_id', $account['organization'], '=')
        ->condition('job_id', $id, '=')
        ->execute();
  }

  drupal_goto('organization-structure');
}

function theme_hr_backend_job_title_structure_form($variables) {
  $form = $variables['element'];

  $rows = array();

  foreach (element_children($form['jobs']) as $id) {
    $form['jobs'][$id]['weight']['#attributes']['class'] = array('item-weight');

    $form['jobs'][$id]['id']['#attributes']['class'] = array('item-id');
    $form['jobs'][$id]['pid']['#attributes']['class'] = array('item-pid');

    $indent = theme('indentation', array('size' => $form['jobs'][$id]['depth']['#value']));
    unset($form['example_items'][$id]['depth']);


    $rows[] = array(
      'data' => array(
        $indent . drupal_render($form['jobs'][$id]['name']) . drupal_render($form['jobs'][$id]['id']) . drupal_render($form['jobs'][$id]['pid']),
        drupal_render($form['jobs'][$id]['weight']),
      ),
      'class' => array('draggable'),
    );
  }

  $header = array(t('Job Title'), t('Weight'));

  $table_id = 'organization-structure';

  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => $table_id, 'class' => array('table', 'table-bordered', 'table-striped')),
  ));

  $output .= drupal_render_children($form);

  drupal_add_tabledrag($table_id, 'match', 'parent', 'item-pid', 'item-pid', 'item-id', FALSE);


  drupal_add_tabledrag($table_id, 'order', 'sibling', 'item-weight');

  return $output;
}

function hr_backend_job_title_structure_get_tree($jobs, $parent, &$items, &$depth) {
  $depth++;

  $job_id = $parent['job_id'];
  $items[$job_id] = array(
    'name' => $jobs[$job_id],
    'id' => $job_id,
    'parent' => $parent['parent'],
    'weight' => $parent['weight'],
    'depth' => $depth,
  );
  // Retrieve each of the children belonging to this parent.
  $children = db_select('hr_organization_structure', 'organization_structure')
          ->fields('organization_structure', array('job_id', 'parent', 'weight'))
          ->condition('parent', $job_id, '=')
          ->orderBy('weight', 'ASC')
          ->execute()->fetchAllAssoc('job_id', PDO::FETCH_ASSOC);

  foreach ($children as $child) {
    if (!in_array($child['job_id'], array_keys($items))) {
      hr_backend_job_title_structure_get_tree($jobs, $child, $items, $depth);
    }
  }
  $depth--;
}
