<?php

function hrm_backend_entity_pdf_generate_pdf($html, $filename, $dest, $header = '', $footer = '', $serial = '') {
  require_once 'sites/all/libraries/mpdf57/mpdf.php';

  // Creating Instance of mPDF Class Library.
  $mpdf = new mPDF(
      '', array(210, 297), '', //$font_size,
      '', //$font_style,
      10, //$margin_left,
      10, //$margin_right,
      35, //$margin_top,
      16, //$margin_bottom,
      9, //$margin_header,
      9 //$margin_footer
  );

  // set document DPI
  $mpdf->dpi = 96;

  // Set image DPI
  $mpdf->img_dpi = 96;

  if (isset($header) && $header != '') {
    $mpdf->SetHTMLHeader($header);
  }

  if ($footer == '') {
    $footer = '<div class="col-6">{PAGENO}</div>';
  }
  else {
    $footer = '<div class="col-6">' . $footer . '</div>';
  }
  if ($serial != '') {
    $footer .= '<div class="document-serial col-6-right">' . $serial . '</div>';
  }

  $mpdf->SetHTMLFooter($footer);

  $stylesheet_content = file_get_contents(drupal_get_path('theme', 'pro_ui') . '/css/pdf.css');
  $mpdf->WriteHTML($stylesheet_content, 1);

  // Writing html content for pdf buffer.
  $mpdf->WriteHTML($html);

  if (!file_prepare_directory($dest)) {
    file_prepare_directory($dest, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  }

  $path = $dest . '/' . $filename . '.pdf';
  $mpdf->Output($path, 'F');
  
  return TRUE;
}

function hrm_backend_entity_pdf_generate_landscape_pdf($html, $filename, $dest, $header = '', $footer = '', $serial = '') {
  require_once 'sites/all/libraries/mpdf57/mpdf.php';

  // Creating Instance of mPDF Class Library.
  $mpdf = new mPDF(
      '', array(210, 297), '', //$font_size,
      '', //$font_style,
      10, //$margin_left,
      10, //$margin_right,
      30, //$margin_top,
      10, //$margin_bottom,
      5, //$margin_header,
      5, //$margin_footer
      'L'
  );

  // set document DPI
  $mpdf->dpi = 96;

  // Set image DPI
  $mpdf->img_dpi = 96;

  if (isset($header) && $header != '') {
    $mpdf->SetHTMLHeader($header);
  }

  if ($footer == '') {
    $footer = '<div class="col-6">{PAGENO}</div>';
  }
  else {
    $footer = '<div class="col-6">' . $footer . '</div>';
  }
  if ($serial != '') {
    $footer .= '<div class="document-serial col-6-right">' . $serial . '</div>';
  }

  $mpdf->SetHTMLFooter($footer);

  $stylesheet_content = file_get_contents(drupal_get_path('theme', 'pro_ui') . '/css/pdf2.css');
  $mpdf->WriteHTML($stylesheet_content, 1);

  // Writing html content for pdf buffer.
  $mpdf->WriteHTML($html);

  if (!file_prepare_directory($dest)) {
    file_prepare_directory($dest, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  }

  $path = $dest . '/' . $filename . '.pdf';
  $mpdf->Output($path, 'F');
  
  return TRUE;
}