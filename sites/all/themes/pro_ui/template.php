<?php

/**
 * @file
 * template.php
 */
function pro_ui_preprocess_page(&$variables) {
  $variables['page_wrapper_clases'] = array();
 
  if ((arg(0) == 'user' && is_numeric(arg(1)))) {
    $account = hrm_basic_account_get_info(arg(1));
    $variables['title'] = $account['name'];
    drupal_set_title($account['name']);
  }

  $contry_flag_path = drupal_get_path('theme', 'pro_ui') . '/images/flags/' . variable_get(MED_VAR_ADMIN_IMPACT_COUNTRY_FLAG_CLASS, 'not-fount') . '.png';
  if (file_exists($contry_flag_path)) {
    $variables['country_flag'] = theme('image', array('path' => $contry_flag_path, 'attributes' => array('class' => array('sidebar-brand-country-image'))));
  }

  if (arg(0) == 'dashboard') {
    $variables['page_wrapper_clases'][] = 'page-loading';

    $pre_loader_background = variable_get(MED_VAR_ADMIN_INTERFACE_PRE_LOADER_BACKGROUND, array());
    $page_loading_logo = theme('image', array('path' => MED_LOGO_MED_RIMARY));

    $variables['pre_loader_content'] = '
    <div class="preloader" style="background-image: url(' . $pre_loader_background['url'] . ');">   
      <div class="loader-container">
        <div class="loader-logo">' . $page_loading_logo . '</div>
        <div class="loader-spinner"></div>
      </div>      
    </div>';
  }
  //Changelog content
//  $changelog_html_id = hrm_basic_changelog_get_html_id();
//  if ($changelog_html_id) {
//    $variables['changelog_content'] = hrm_basic_changelog_get_html_content();
//  }
}

/*
 * hook_theme - to create custom pages for some forms
 */

function pro_ui_theme() {
  return array(
    /* ----------------FAPI---------------- */
    'hrm_table_field' => array(
      'render element' => 'form',
    ),
    /* ----------------Entity---------------- */
    //Managment
    'hrm_backend_entity_forms_approve_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/entity-approve-form',
      'render element' => 'form',
    ),
    'hrm_backend_entity_forms_reject_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/entity-reject-form',
      'render element' => 'form',
    ),
    'hrm_backend_entity_forms_open_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/entity-open-form',
      'render element' => 'form',
    ),
    'hrm_backend_entity_forms_close_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/entity-close-form',
      'render element' => 'form',
    ),
    'hrm_backend_entity_forms_activate_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/entity-activate-form',
      'render element' => 'form',
    ),
    'hrm_backend_entity_forms_block_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/entity-block-form',
      'render element' => 'form',
    ),
    'hrm_backend_entity_forms_cancel_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/entity-cancel-form',
      'render element' => 'form',
    ),
    'hrm_backend_entity_attendance_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/entity-attendance-form',
      'render element' => 'form',
    ),
    //Picture
    'hrm_backend_entity_photo_add_photos_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/photo/photo-add-form',
      'render element' => 'form',
    ),
    'hrm_backend_entity_photo_edit_photo_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/photo/photo-edit-form',
      'render element' => 'form',
    ),
    'hrm_backend_entity_picture_edit_entity_pictures_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/photo/entity-pictures-edit-form',
      'render element' => 'form',
    ),
    //Video
    'hrm_backend_entity_video_add_video_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/video/video-add-form',
      'render element' => 'form',
    ),
    'hrm_backend_entity_video_edit_video_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/video/video-edit-form',
      'render element' => 'form',
    ),
    //Document
    'hrm_backend_entity_document_add_documents_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/document/document-add-form',
      'render element' => 'form',
    ),
    'hrm_backend_entity_document_edit_document_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/document/document-edit-form',
      'render element' => 'form',
    ),
    //Financial Cheque
    'hrm_backend_entity_financial_proposal_cheque_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/financial/cheque-form',
      'render element' => 'form',
    ),
    //AFR
    'hrm_backend_entity_afr_close_afr_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/afr/close-afr-form',
      'render element' => 'form',
    ),
    'hrm_backend_entity_afr_reject_afr_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/afr/reject-afr-form',
      'render element' => 'form',
    ),
    'hrm_backend_entity_afr_remaining_receipt_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/afr/remaining-receipt-afr-form',
      'render element' => 'form',
    ),
    'hrm_backend_entity_afr_item_tags_form' => array(
      'arguments' => array('form' => NULL), 'render element' => 'form',
      'template' => 'templates/modules/hrm_backend/entity/afr/afr-item-tags-form',
    ),
    'hrm_backend_entity_afr_complete_afr_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/afr/complete-afr-form',
      'render element' => 'form',
    ),
    //Tracker
    'hrm_backend_entity_tracker_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/tracker/tracker-form',
      'render element' => 'form',
    ),
    //Conclusion
    'hrm_backend_entity_conclusion_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/entity-conclusion-form',
      'render element' => 'form',
    ),
    //Rate
    'hrm_backend_entity_rate_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/entity-rate-form',
      'render element' => 'form',
    ),
    //Manage Staff
    'hrm_backend_entity_forms_staff_manage_staff_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/entity-staff-form',
      'render element' => 'form',
    ),
    //Collaboration
    'hrm_backend_entity_collab_post_add_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/collab/collab-post-add-form',
      'render element' => 'form',
    ),
    'hrm_backend_entity_collab_post_edit_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/entity/collab/collab-post-edit-form',
      'render element' => 'form',
    ),
    /*
     * ========================================
     * Workspace
     * ========================================
     */
    /*
     * ========================================
     * Administration 
     * ========================================
     */
    /* ----------------Offices---------------- */
    'hrm_backend_office_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/admin/office/office-form',
      'render element' => 'form',
    ),
    /* ----------------Venues---------------- */
    'hrm_backend_venue_add_venue_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/admin/venue/venue-form',
      'render element' => 'form',
    ),
    'hrm_backend_venue_edit_venue_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/admin/venue/venue-form',
      'render element' => 'form',
    ),
    /* ----------------International Days---------------- */
    'hrm_backend_international_day_add_international_day_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/admin/international_day/international-day-form',
      'render element' => 'form',
    ),
    'hrm_backend_international_day_edit_international_day_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/admin/international_day/international-day-form',
      'render element' => 'form',
    ),
    /* ----------------Focal points---------------- */
    'hrm_backend_focal_point_add_focal_point_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/admin/focal_point/focal-point-form',
      'render element' => 'form',
    ),
    'hrm_backend_focal_point_edit_focal_point_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/admin/focal_point/focal-point-form',
      'render element' => 'form',
    ),
    /*
     * ========================================
     * Project Management 
     * ========================================
     */
    /* ----------------Projects---------------- */
    'hrm_backend_project_add_project_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/pm/project/project-form',
      'render element' => 'form',
    ),
    'hrm_backend_project_edit_project_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/pm/project/project-form',
      'render element' => 'form',
    ),
    'hrm_backend_project_branding_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/pm/project/project-branding-form',
      'render element' => 'form',
    ),
    //LogFrame
    'hrm_backend_logframe_logframe_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/pm/project/logframe/logframe-form',
      'render element' => 'form',
    ),
    'hrm_backend_logframe_edit_item_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/pm/project/logframe/logframe-item-edit-form',
      'render element' => 'form',
    ),
    'hrm_backend_logframe_revision_revert_revision_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/pm/project/logframe/logframe-revision-revert-form',
      'render element' => 'form',
    ),
    'hrm_backend_logframe_logframe_label_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/pm/project/logframe/logframe-label-form',
      'render element' => 'form',
    ),
    'hrm_backend_logframe_logframe_budget_outcome_output_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/pm/project/logframe/logframe-budget-outcome-output-form',
      'render element' => 'form',
    ),
    'hrm_backend_logframe_reject_logframe_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/pm/project/logframe/logframe-reject-form',
      'render element' => 'form',
    ),
    //PMF
    'hrm_backend_pmf_indicator_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/pm/project/pmf/pmf-indicator-form',
      'render element' => 'form',
    ),
    'hrm_backend_pmf_delete_indicator_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/pm/project/pmf/pmf-indicator-delete-form',
      'render element' => 'form',
    ),
    'hrm_backend_pmf_lock_indicator_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/pm/project/pmf/pmf-indicator-lock-form',
      'render element' => 'form',
    ),
    'hrm_backend_pmf_unlock_indicator_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/pm/project/pmf/pmf-indicator-unlock-form',
      'render element' => 'form',
    ),
    'hrm_backend_pmf_indicator_outcome_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/pm/project/pmf/pmf-indicator-outcome-form',
      'render element' => 'form',
    ),
    'hrm_backend_pmf_indicator_outcome_update_value_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/pm/project/pmf/pmf-indicator-outcome-update-value-form',
      'render element' => 'form',
    ),
    /* ----------------Activities---------------- */
    'hrm_backend_activity_activity_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/pm/activity/activity-form',
      'render element' => 'form',
    ),
    'hrm_backend_activity_attendance_sheet_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/pm/activity/activity-attendance-sheet-form',
      'render element' => 'form',
    ),
    /*
     * ========================================
     * Beneficiaries
     * ========================================
     */
    /* ----------------Household---------------- */
    'hrm_backend_household_transfer_household_form' => array(
      'template' => 'templates/modules/hrm_backend/benef/household/household-transfer-form',
      'arguments' => array('form' => NULL), 'render element' => 'form'
    ),
    'hrm_backend_household_submit_household_form' => array(
      'template' => 'templates/modules/hrm_backend/benef/household/household-submit-form',
      'arguments' => array('form' => NULL), 'render element' => 'form'
    ),
    'hrm_backend_household_delete_household_form' => array(
      'template' => 'templates/modules/hrm_backend/benef/household/household-delete-form',
      'arguments' => array('form' => NULL), 'render element' => 'form'
    ),
    //members
    'hrm_backend_household_member_activate_form' => array(
      'template' => 'templates/modules/hrm_backend/benef/household/household-member-activate-form',
      'arguments' => array('form' => NULL), 'render element' => 'form'
    ),
    'hrm_backend_household_member_block_form' => array(
      'template' => 'templates/modules/hrm_backend/benef/household/household-member-block-form',
      'arguments' => array('form' => NULL), 'render element' => 'form'
    ),
    'hrm_backend_household_member_delete_form' => array(
      'template' => 'templates/modules/hrm_backend/benef/household/household-member-delete-form',
      'arguments' => array('form' => NULL), 'render element' => 'form'
    ),
    'hrm_backend_household_bulk_manage_submitted_form' => array(
      'template' => 'templates/modules/hrm_backend/benef/household/household-bulk-manage-submitted',
      'arguments' => array('form' => NULL), 'render element' => 'form'
    ),
    /* ----------------Case---------------- */
    'hrm_backend_case_add_case_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/case/case-form',
      'render element' => 'form',
    ),
    'hrm_backend_case_edit_case_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/case/case-form',
      'render element' => 'form',
    ),
    'hrm_backend_case_transfer_case_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/case/case-transfer-form',
      'render element' => 'form',
    ),
    'hrm_backend_case_close_case_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/case/case-close-form',
      'render element' => 'form',
    ),
    'hrm_backend_case_archive_case_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/case/case-archive-form',
      'render element' => 'form',
    ),
    //Case Service Forms
    'hrm_backend_case_service_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/case/case-service-form',
      'render element' => 'form',
    ),
    'hrm_backend_case_service_delete_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/case/case-service-delete-form',
      'render element' => 'form',
    ),
    'hrm_backend_case_service_update_status_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/case/case-service-update-status-form',
      'render element' => 'form',
    ),
    //Service Followup Forms
    'hrm_backend_case_service_followup_add_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/case/case-service-followup-form',
      'render element' => 'form',
    ),
    'hrm_backend_case_service_followup_edit_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/case/case-service-followup-form',
      'render element' => 'form',
    ),
    'hrm_backend_case_service_followup_delete_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/case/case-service-followup-delete-form',
      'render element' => 'form',
    ),
    //Case Assessment Forms
    'hrm_backend_case_complete_in_dpth_assessment_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/case/assessment/case-assessment-complete-form',
      'render element' => 'form',
    ),
    //Case Cash assistance
    'hrm_backend_case_cash_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/case/case-cash-form',
      'render element' => 'form',
    ),
    'hrm_backend_case_cash_exclude_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/case/case-cash-exclude-form',
      'render element' => 'form',
    ),
    'hrm_backend_case_cash_include_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/case/case-cash-include-form',
      'render element' => 'form',
    ),
    //Case Phone Call
    'hrm_backend_case_call_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/case/case-call-form',
      'render element' => 'form',
    ),
    /* ----------------Cash Assistance---------------- */
    'hrm_backend_cash_assist_submit_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/cash_assist/cash-assist-submit-form',
      'render element' => 'form',
    ),
    'hrm_backend_cash_assist_close_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/cash_assist/cash-assist-close-form',
      'render element' => 'form',
    ),
    'hrm_backend_cash_assist_line_exclude_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/cash_assist/cash-assist-line-exclude-form',
      'render element' => 'form',
    ),
    'hrm_backend_cash_assist_line_include_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/cash_assist/cash-assist-line-include-form',
      'render element' => 'form',
    ),
    'hrm_backend_cash_assist_line_update_amount_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/cash_assist/cash-assist-line-update-amount-form',
      'render element' => 'form',
    ),
    'hrm_backend_cash_assist_line_update_payment_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/cash_assist/cash-assist-line-update-payment-form',
      'render element' => 'form',
    ),
    'hrm_backend_cash_assist_line_refund_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/cash_assist/cash-assist-line-refund-form',
      'render element' => 'form',
    ),
    /* ----------------Protections---------------- */
    'hrm_backend_prot_referral_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/prot/prot-referral-form',
      'render element' => 'form',
    ),
    'hrm_backend_prot_referral_update_status_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/case/prot/prot-referral-update-status-form',
      'render element' => 'form',
    ),
    /*
     * ========================================
     * CRM
     * ========================================
     */
    /* ----------------Service Mapping---------------- */
    'hrm_backend_service_mapping_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/crm/service_mapping/service-mapping-form',
      'render element' => 'form',
    ),
    /*
     * ========================================
     * Humman Resources
     * ========================================
     */
    /* ----------------Users---------------- */
    'hrm_backend_user_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/hr/user/user-form',
      'render element' => 'form',
    ),
    'hrm_backend_user_activate_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/hr/user/user-activate-form',
      'render element' => 'form',
    ),
    'hrm_backend_user_block_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/hr/user/user-block-form',
      'render element' => 'form',
    ),
    'hrm_backend_user_emp_of_quarter_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/hr/user/user-emp-of-quarter-form',
      'render element' => 'form',
    ),
    /* ----------------Training---------------- */
    //Trainer
    'hrm_backend_training_add_trainer_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/hr/training/trainer/trainer-add-form',
      'render element' => 'form',
    ),
    'hrm_backend_training_edit_trainer_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/hr/training/trainer/trainer-edit-form',
      'render element' => 'form',
    ),
    //Training
    'hrm_backend_training_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/hr/training/training-form',
      'render element' => 'form',
    ),
    /*
     * ========================================
     * Finance
     * ========================================
     */
    /* ----------------Chart of accounts---------------- */
    'hrm_backend_code_project_code_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/finance/code/project-code-form',
      'render element' => 'form',
    ),
    'hrm_backend_code_donor_code_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/finance/code/donor-code-form',
      'render element' => 'form',
    ),
    'hrm_backend_code_activity_code_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/finance/code/activity-code-form',
      'render element' => 'form',
    ),
    'hrm_backend_code_account_code_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/finance/code/account-code-form',
      'render element' => 'form',
    ),
    /*
     * ========================================
     * Communications
     * ========================================
     */
    /* ----------------Success Story---------------- */
    'hrm_backend_success_story_success_story_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/communications/success_story/success-story-form',
      'render element' => 'form',
    ),
    /*
     * ========================================
     * Logistics forms
     * ========================================
     */
    /*
     * ========================================
     * Resources
     * ========================================
     */
    /* ---------------- Agenda templates ---------------- */
    'hrm_backend_resource_agenda_template_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_backend/resources/agenda_templates/agenda-template-form',
      'render element' => 'form',
    ),
    /*
     * ========================================
     * Medair Forms module
     * ========================================
     */
    /* ----------------Entity---------------- */
    //Entity review
    'hrm_forms_entity_review_question_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_forms/entity/review/forms-entity-review-question-form',
      'render element' => 'form',
    ),
    /*
     * ========================================
     * Medair Response Collect module
     * ========================================
     */
    /* ----------------Survey---------------- */
    'hrm_collect_survey_build_survey_info_section_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_collect/survey/survey-info-form',
      'render element' => 'form',
    ),
    'hrm_collect_survey_build_survey_question_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_collect/survey/survey-question-form',
      'render element' => 'form',
    ),
    'hrm_collect_survey_build_survey_collector_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_collect/survey/survey-collectors-form',
      'render element' => 'form',
    ),
    'hrm_collect_survey_build_survey_summary_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_collect/survey/survey-summary-form',
      'render element' => 'form',
    ),
    //Survey managment forms
    'hrm_collect_survey_reject_survey_form' => array(
      'arguments' => array(
        'form' => NULL,
      ),
      'template' => 'templates/modules/hrm_collect/survey/survey-reject-form',
      'render element' => 'form',
    ),
  );
}

function pro_ui_menu_tree__secondary(&$variables) {
  return '<ul class="nav navbar-nav-custom pull-right">' . $variables['tree'] . '</ul>';
}

function pro_ui_menu_local_tasks(&$variables) {
  $output = '';

  if (!empty($variables['primary'])) {
    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>';
    $variables['primary']['#prefix'] .= '<div class="nav-tabs-wrapper"><ul class="tabs--primary nav nav-tabs">';
    $variables['primary']['#suffix'] = '</ul></div>';
    $output .= drupal_render($variables['primary']);
  }

  if (!empty($variables['secondary'])) {
    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>';
    $variables['secondary']['#prefix'] .= '<ul class="tabs--secondary pagination pagination-sm">';
    $variables['secondary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}

/**
 * Overrides theme_status_messages().
 */
function pro_ui_status_messages($variables) {
  $display = $variables['display'];
  $output = '';
  $status_class = array(
    'status' => 'success',
    'error' => 'danger',
    'warning' => 'warning',
    'info' => 'info',
  );

  foreach (drupal_get_messages($display) as $type => $messages) {
    $message_content = '';
    if (count($messages) > 1) {

      $message_content .= ' <ul>';
      foreach ($messages as $message) {
        $message_content .= '  <li>' . htmlspecialchars_decode(strip_tags($message, '<br><a><b><strong>')) . '</li>';
      }
      $message_content .= ' </ul>';
    }
    else {
      $message_content .= strip_tags($messages[0], '<br><a><b><strong>');
    }
    $output .= '<div class="notify-message" data-notify-message="' . $message_content . '" data-notify-type="' . $status_class[$type] . '"></div>';
  }
  return $output;
}

function pro_ui_js_alter(&$js) {
  // Define path to old jQuery Form plugin, alter to your needs.
  $old_path = drupal_get_path('module', 'jquery_update') . '/replace/misc/jquery.form.min.js';

  // The script does not get loaded on every page, so check the $js array.
  if (isset($js[$old_path])) {
    // Define path to newer jQuery Form plugin, alter to your needs.
    $new_path = drupal_get_path('theme', 'pro_ui') . '/js/jquery.form.min.js';

    // Copy data from old javascript to new and remove old javascript.
    $js[$new_path] = $js[$old_path];
    unset($js[$old_path]);

    // Set new path as data property and alter version number.
    $js[$new_path]['data'] = $new_path;
    $js[$new_path]['version'] = '3.51.0';
  }
}

function pro_ui_get_user_login_form() {
  $form = drupal_get_form('user_login');
  $rendered_form = render($form);
  return '<div class="col-xs-12">' . pro_ui_status_messages(array('display' => '')) . '</div>' . $rendered_form;
}

function pro_ui_get_user_password_form() {
  $form = drupal_get_form('user_pass');
  $rendered_form = render($form);
  return '<div class="col-xs-12">' . pro_ui_status_messages(array('display' => '')) . '</div>' . $rendered_form;
}

/**
 * Returns HTML for a list of users.
 * Used for who's online
 */
function pro_ui_user_list($variables) {
  $users = $variables['users'];
  $title = $variables['title'];
  $items = array();

  if (!empty($users)) {
    foreach ($users as $user) {
      if ($user->uid == 1) {
        continue;
      }
      $items[] = hrm_basic_user_get_name($user->uid);
    }
  }
  return theme('item_list', array('items' => $items, 'title' => $title));
}

function pro_ui_file_upload_help($variables) {
  $description = $variables['description'];
  $upload_validators = $variables['upload_validators'];

  $descriptions = array();

  if (strlen($description)) {
    $descriptions[] = $description;
  }
  if (isset($upload_validators['file_validate_size'])) {
    $descriptions[] = t('Files must be less than !size.', array('!size' => '<strong>' . format_size($upload_validators['file_validate_size'][0]) . '</strong>'));
  }
  if (isset($upload_validators['file_validate_extensions'])) {
    $extensions = check_plain($upload_validators['file_validate_extensions'][0]);
    $descriptions[] = t('Valid file extensions: !extensions', array('!extensions' => '<strong>' . str_replace(' ', ', ', $extensions) . '</strong>'));
  }
  if (isset($upload_validators['file_validate_image_resolution'])) {
    $max = $upload_validators['file_validate_image_resolution'][0];
    $min = $upload_validators['file_validate_image_resolution'][1];
    if ($min && $max && $min == $max) {
      $descriptions[] = t('Images must be exactly !size pixels.', array('!size' => '<strong>' . $max . '</strong>'));
    }
    elseif ($min && $max) {
      $descriptions[] = t('Images must be between !min and !max pixels.', array('!min' => '<strong>' . $min . '</strong>', '!max' => '<strong>' . $max . '</strong>'));
    }
    elseif ($min) {
      $descriptions[] = t('Images must be larger than !min pixels.', array('!min' => '<strong>' . $min . '</strong>'));
    }
    elseif ($max) {
      $descriptions[] = t('Images must be smaller than !max pixels.', array('!max' => '<strong>' . $max . '</strong>'));
    }
  }

  return implode('<br />', $descriptions);
}

/*
 * 
 */

function pro_ui_link($variables) {
  $download_attribute = FALSE;
  if (isset($variables['options']['attributes']['download'])) {
    $download_attribute = TRUE;
    unset($variables['options']['attributes']['download']);
  }

  return '<a href="' . check_plain(url($variables['path'], $variables['options'])) . '"' . drupal_attributes($variables['options']['attributes']) . ($download_attribute ? ' download' : '') . '>' . ($variables['options']['html'] ? $variables['text'] : check_plain($variables['text'])) . '</a>';
}

/*
 * FAPI trmplates
 */

function pro_ui_hrm_table_field(&$variables) {
  // Get the useful values.
  $form = $variables['form'];
  $rows = $form['rows'];
  $header = $form['#header'];
  $attributes = (isset($form['#attributes']) ? $form['#attributes'] : array());

  $draggable_table = FALSE;
  if (isset($attributes['class']) && !empty($attributes['class']) && in_array('draggable-table', $attributes['class'])) {
    $draggable_table = TRUE;
    if (!isset($attributes['id'])) {
      $attributes['id'] = 'draggable-table-' . hrm_basic_string_get_random_string(10) . '-' . time();
    }
  }
  // Setup the structure to be rendered and returned.
  $content = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => array(),
    '#attributes' => $attributes,
    '#sticky' => FALSE,
  );

  // Traverse each row.  @see element_chidren().
  foreach (element_children($rows) as $row_index) {
    $row = array();
    $row_class = array();

    if (isset($rows[$row_index]['#class'])) {
      $row_class = $rows[$row_index]['#class'];
      unset($rows[$row_index]['#class']);
    }


    // Traverse each column in the row.  @see element_children().
    foreach (element_children($rows[$row_index]) as $col_index) {

      $cell_class = array();
      if (isset($rows[$row_index][$col_index]['#class'])) {
        $cell_class = $rows[$row_index][$col_index]['#class'];
        unset($rows[$row_index][$col_index]['#class']);
      }
      $cell_data = drupal_render($rows[$row_index][$col_index]);
      // Render the column form element.
      $row[] = array('data' => $cell_data, 'class' => $cell_class);
    }
    if ($draggable_table) {
      $row_class[] = 'draggable';
    }
    // Add the row to the table.    
    $content['#rows'][] = array('data' => $row, 'class' => $row_class);
  }

  if (isset($form['add_more'])) {
    $row = array(
      'data' => array('data' => drupal_render($form['add_more']),
        'colspan' => count($header),
        'class' => array('table-field-add-more')),
    );
    $content['#rows'][] = array('data' => $row);
  }

  if (isset($attributes['class']) && !empty($attributes['class']) && in_array('draggable-table', $attributes['class'])) {
    drupal_add_tabledrag($attributes['id'], 'order', 'sibling', 'entry-order-weight');
  }

  // Redner the table and return.
  return drupal_render($content);
}

function pro_ui_remove_file_element_process($element, &$form_state, $form) {
  $element = file_managed_file_process($element, $form_state, $form);
  $element['upload_button']['#access'] = FALSE;
  return $element;
}

function pro_ui_table($variables) {
  $header = $variables['header'];
  $rows = $variables['rows'];
  $attributes = $variables['attributes'];
  $caption = $variables['caption'];
  $colgroups = $variables['colgroups'];
  $sticky = $variables['sticky'];
  $empty = $variables['empty'];

  // Add sticky headers, if applicable.
  if (count($header) && $sticky) {
    drupal_add_js('misc/tableheader.js');
    // Add 'sticky-enabled' class to the table to identify it for JS.
    // This is needed to target tables constructed by this function.
    $attributes['class'][] = 'sticky-enabled';
  }
  $output = '';

  if (isset($variables['responsive']) && $variables['responsive']) {
    $output .= '<div class="table-responsive">';
  }

  $output .= '<table ' . drupal_attributes($attributes) . ">\n";

  if (isset($caption)) {
    $output .= '<caption>' . $caption . "</caption>\n";
  }

  // Format the table columns:
  if (count($colgroups)) {
    foreach ($colgroups as $number => $colgroup) {
      $attributes = array();

      // Check if we're dealing with a simple or complex column
      if (isset($colgroup['data'])) {
        foreach ($colgroup as $key => $value) {
          if ($key == 'data') {
            $cols = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $cols = $colgroup;
      }

      // Build colgroup
      if (is_array($cols) && count($cols)) {
        $output .= ' <colgroup' . drupal_attributes($attributes) . '>';
        $i = 0;
        foreach ($cols as $col) {
          $output .= ' <col' . drupal_attributes($col) . ' />';
        }
        $output .= " </colgroup>\n";
      }
      else {
        $output .= ' <colgroup' . drupal_attributes($attributes) . " />\n";
      }
    }
  }

  // Add the 'empty' row message if available.
  if (!count($rows) && $empty) {
    $header_count = 0;
    foreach ($header as $header_cell) {
      if (is_array($header_cell)) {
        $header_count += isset($header_cell['colspan']) ? $header_cell['colspan'] : 1;
      }
      else {
        $header_count++;
      }
    }
    $rows[] = array(array('data' => $empty, 'colspan' => $header_count, 'class' => array('empty', 'message')));
  }

  // Format the table header:
  if (count($header)) {
    if (isset($variables['multi_header']) && $variables['multi_header']) {
      $ts = array();

      $output .= '<thead>';
      foreach ($header as $header_row) {
        $output .= '<tr>';
        foreach ($header_row as $cell) {
          $output .= _theme_table_cell($cell, TRUE);
        }
        $output .= '</tr>';
      }
      // Using ternary operator to close the tags based on whether or not there are rows
      $output .= '</thead>';
    }
    else {
      $ts = tablesort_init($header);
      // HTML requires that the thead tag has tr tags in it followed by tbody
      // tags. Using ternary operator to check and see if we have any rows.
      $output .= (count($rows) ? ' <thead><tr>' : ' <tr>');
      foreach ($header as $cell) {
        $cell = tablesort_header($cell, $header, $ts);
        $output .= _theme_table_cell($cell, TRUE);
      }
      // Using ternary operator to close the tags based on whether or not there are rows
      $output .= (count($rows) ? " </tr></thead>\n" : "</tr>\n");
    }
  }
  else {
    $ts = array();
  }

  // Format the table rows:
  if (count($rows)) {
    $output .= "<tbody>\n";
    $flip = array('even' => 'odd', 'odd' => 'even');
    $class = 'even';
    foreach ($rows as $number => $row) {
      // Check if we're dealing with a simple or complex row
      if (isset($row['data'])) {
        $cells = $row['data'];
        $no_striping = isset($row['no_striping']) ? $row['no_striping'] : FALSE;

        // Set the attributes array and exclude 'data' and 'no_striping'.
        $attributes = $row;
        unset($attributes['data']);
        unset($attributes['no_striping']);
      }
      else {
        $cells = $row;
        $attributes = array();
        $no_striping = FALSE;
      }
      if (count($cells)) {
        // Add odd/even class
        if (!$no_striping) {
          $class = $flip[$class];
          $attributes['class'][] = $class;
        }

        // Build row
        $output .= ' <tr' . drupal_attributes($attributes) . '>';
        $i = 0;
        foreach ($cells as $cell) {
          $cell = tablesort_cell($cell, $header, $ts, $i++);
          $output .= _theme_table_cell($cell);
        }
        $output .= " </tr>\n";
      }
    }
    $output .= "</tbody>\n";
  }

  if (isset($variables['footer']) && !empty($variables['footer'])) {
    $output .= '<tfoot><tr>';
    foreach ($variables['footer'] as $footer_cell) {
      $output .= _theme_table_cell($footer_cell);
    }
    $output .= '</tr></tfoot>';
  }

  $output .= "</table>\n";

  if (isset($variables['responsive']) && $variables['responsive']) {
    $output .= '</div>';
  }

  return $output;
}

function pro_ui_system_settings_form($variables) {
  $variables['form']['actions']['submit']['#prefix'] = '<div class="form-group form-actions">';
  $variables['form']['actions']['submit']['#suffix'] = '</div>';
  $variables['form']['actions']['submit']['#attributes']['class'][] = 'btn-sm';

  return drupal_render_children($variables['form']);
}
